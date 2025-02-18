#include <Dev/PWM/stm32f4xx_tim.h>
#include <Dev/TB6612/tb6612_port.h>
#include <Project.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <misc.h>
#include <usart/Serial.h>
extern Stde_DataTypeDef USART3_DataBuff, UART5_DataBuff, UART4_DataBuff;

extern TaskHandle_t *Task3_Project_Display_Mode_1_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_1_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_3_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_4_Handle;
extern TaskHandle_t *Task4_LEDPlayR_Handle;
extern TaskHandle_t *Task4_LEDPlayY_Handle;

uint8_t TaskDeletSign;

/// @brief 无条件固定运行次数的函数接口
/// @param Fp 运行函数
/// @param Count 运行函数
/// @return
uint8_t Temp_Run(void *(Fp)(), uint8_t Count) {
    static uint8_t i = 0;
    if (i < Count) {
        Fp();
    }
}

/// @brief get the value of encoder on TIM4 and TIM5
void Project_LIB_Get_Encoder_Value(uint16_t *value1, uint16_t *value2) {
    *value1 = TIM4->CNT;
    *value2 = TIM5->CNT;
}

void Project_LIB_TIM3_Init(uint8_t ms) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1000 * ms - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    printf("初始化定时器\n");
}

uint8_t MotorStrat_1 =
    1;  //  电机启动最高优先级：电源控制
        //  默认插上跳线帽为标准功耗，开启电机，拔下跳线帽为低功耗，关闭电机
uint8_t MotorStrat_2 = 0;  //  电机启动第二优先级：药品放置
                           //  默认放置药品就开启电机，拿走药品就关闭电机
uint8_t MotorStrat_3 = 0;  //  电机启动第三优先级：巡线识别
                           //  默认巡线识别就开启电机，停止巡线识别就关闭电机

/// @brief Motor load function for left and right
void Project_LIB_Motor_Load(int32_t leftMotor, int32_t RightMotor) {
    if (MotorStrat_1) {
        if (MotorStrat_2) {
            if (MotorStrat_3) {
                if (leftMotor < 0) {
                    LeftBackward Motor->Left = -leftMotor;
                } else if (leftMotor > 0) {
                    LeftForward Motor->Left = leftMotor;
                } else {
                    Motor->Left = 0;
                }
                if (RightMotor < 0) {
                    RightBackward Motor->Right = -RightMotor;
                } else if (RightMotor > 0) {
                    RightForward Motor->Right = RightMotor;
                } else {
                    Motor->Right = 0;
                }
            } else {
                Motor->Left = 0;
                Motor->Right = 0;
                AllStop
            }
        } else {
            Motor->Left = 0;
            Motor->Right = 0;
            AllStop
        }
    } else {
        Motor->Left = 0;
        Motor->Right = 0;
        AllStop
    }
}

uint8_t CamerData[4];
uint8_t DataLock = 0;  // 规定数据存储次数，静态变量实现自锁与解锁

uint8_t RLContrl = 0;

/// @brief 将从串口读出的数据保存到Data中
/// @return -2 相等返回 -1 返回出现扫描空挡 0，1 返回当前存储的位置
uint8_t Data_Save_from_Camer() {
    if (!DataLock) {
        static uint8_t Temp, i;
        if (Temp == K210Data) {
            if (i == 1) {            // 存入一个数字，选择性锁定存储
                if (MotorStrat_2) {  // 此时若电机被启动，直接强行锁定
                    DataLock = 1;
                    if (CamerData[0] == 1) {
                        RLContrl = 1;
                    } else {
                        RLContrl = 2;
                    }
                }
            }
            return -2;  //  上次和这次数据一样
        }
        Temp = K210Data;              // 刷新临时数据
        if (Temp) {                   // 如果识别到了数字
            if (!CamerData[i]) {      // 如果缓冲数组目前为止为空
                CamerData[i] = Temp;  // 填入数字
                if (!i) {             // 返回0闪红灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Red_LED", 1024,
                                1, 10, Task4_LEDPlayR_Handle);
                } else {  // 返回1闪黄灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Yellow_LED",1024,
                                2, 10, Task4_LEDPlayY_Handle);
                }
                i++;
                if (i == 2) {  // 存入两个数字，直接锁定数据存入
                    DataLock = 1;
                }
                return i;
            }
        } else {        // 程序运行到这里，说明出现了扫描空挡
            return -1;  // 返回3只会出现在，第一次出现扫描空挡的瞬间
        }
    }
}

/// @brief 将从串口识别到的数字进行识别配对
/// @return
uint8_t Data_Get_from_Camer() {
    if (DataLock) {
        static uint8_t n = 0;  // 删除任务的信号只发送一次
        if (!n) {
            TaskDeletSign = 2;
            n++;
        }
        if (!TaskDeletSign) {      // 删除任务已完成
            static uint8_t i = 0;  // 创建任务的函数也只调用一次
            if (!i) {
                xTaskCreate((TaskFunction_t)Task3_Project_Display,
                            "DisPlay_Camer", 1024, 21, 10,
                            Task3_Project_Display_Mode_2_1_Handle);
                i++;
            }
            static uint8_t Temp = 0;
            // 首先判断，是存了一个还是两个数字
            if (CamerData[1]) {
            }
        } else {
            return 1;  // 删除任务未完成
        }
    }
}
