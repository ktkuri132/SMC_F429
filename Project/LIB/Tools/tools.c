#include <Dev/PWM/stm32f4xx_tim.h>
#include <Dev/TB6612/tb6612_port.h>
#include <Project.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <misc.h>
#include <stdint.h>
#include <stdio.h>
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
uint8_t Temp_Run(void *(Fp)()) {
    static uint8_t i = 0;
    if (!i) {
        Fp();
        i++;
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

void OpenMV_Camera_Callback(Stde_DataTypeDef *DataTypeStruct)
{
    uint8_t Temp_Data =  DataTypeStruct->UART_DATA_TYPE;

    if(Temp_Data == 1){

    }
    else if(Temp_Data == 2){

    }
    else if(Temp_Data == 3){

    }
}


uint8_t CamerData[4];      // [0] 数字1 [1] 坐标1 [2] 数字2 [3] 坐标2
uint8_t SaveDataLock = 0;  // 规定数据存储次数，静态变量实现自锁与解锁
extern uint8_t Key_Value;

/// @brief 将从串口读出的数据保存到CamerData中,原本读取数字是不用获取坐标信息的,但是当时写错了,后来也就没改过来,函数的运行条件是SaveDataLock必须为0
/// @param MotorStrat_2 电机启动条件之一,根据具体需要提供给这个函数
/// @param Key_Value 按键按下的键值,根据具体需要提供给这个函数
/// @param K210Data 从k210摄像头传来的数字数据,根据具体需要提供给这个函数
/// @param K210Site 从k210摄像头传来的坐标数据,根据具体需要提供给这个函数
/// @return <0 未完成
/// @return >0 完成
int8_t Data_Save_from_Camer() {
    static uint8_t i = 0;
    if (!SaveDataLock) {
        static uint8_t TempNum, TempSite;
        if (!K210Data) {
            if (MotorStrat_2 ||
                (Key_Value == 1)) {  // 此时若电机被启动，直接强行锁定
                SaveDataLock = 1;
                return i;
            }
            return -1;
        }
        if (TempNum == K210Data) {
            if (i == 2) {  // 存入一个数字，选择性锁定存储
                if (MotorStrat_2 ||
                    (Key_Value == 1)) {  // 此时若电机被启动，直接强行锁定
                    SaveDataLock = 1;
                }
            }
            return -2;  //  上次和这次数据一样
        }
        TempNum = K210Data;  // 刷新临时数据
        TempSite = K210Site;
        if (TempNum) {                        // 如果识别到了数字
            if (!CamerData[i]) {              // 如果缓冲数组目前为止为空
                CamerData[i] = TempNum;       // 填入数字
                CamerData[i + 1] = TempSite;  // 填入坐标
                if (!i) {                     // 返回0闪红灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Red_LED", 1024,
                                1, 10, Task4_LEDPlayR_Handle);
                } else {  // 返回1闪黄灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Yellow_LED",
                                1024, 2, 10, Task4_LEDPlayY_Handle);
                }
                i += 2;
                if (i == 4) {  // 存入两个数字，直接锁定数据存入
                    SaveDataLock = 2;
                }
                return i;
            }
        } else {        // 程序运行到这里，说明出现了扫描空挡
            return -1;  // 返回3只会出现在，第一次出现扫描空挡的瞬间
        }
    }
    return i;
}

uint8_t CamerVerify[4];

/// @brief 将从串口识别到的数字进行识别配对,函数运行的第一优先级是SaveDataLock必须为1,第二优先级是VerifyDataLock必须为0
/// @return
int8_t Data_Get_from_Camer() {
    static uint8_t VerifyDataLock = 0;
    if (SaveDataLock) {
            static uint8_t m = 0;
            if (K210Data && !m) {
                return -1;
            }
            m = 1;
            // 首先判断，是存了一个还是两个数字
            switch (SaveDataLock) {
                case 1: {
                    if (!K210Data) {
                        return -1;
                    }
                    if (CamerData[0] == K210Data) {
                        CamerVerify[0] = K210Data;
                        if (K210Site > 60) {
                            CamerVerify[1] = 1;
                            return 1;  // 右边
                        } else {
                            CamerVerify[1] = 2;
                            return 2;  // 左边
                        }
                    }
                } break;
                case 2: {
                    if (!K210Data) {
                        return -1;
                    }
                    if (CamerData[0] == K210Data) {
                        CamerVerify[0] = K210Data;
                        if (K210Site > 60) {
                            CamerVerify[1] = 1;
                            return 1;  // 右边
                        } else {
                            CamerVerify[1] = 2;
                            return 2;  // 左边
                        }
                    }
                    if (CamerData[2] == K210Data) {
                        CamerVerify[2] = K210Data;
                        if (K210Site > 60) {
                            CamerVerify[3] = 1;
                            return 1;  // 右边
                        } else {
                            CamerVerify[3] = 2;
                            return 2;  // 左边
                        }
                    }
                }
                default:
                    break;
            }

            return -1;  // 删除任务未完成
        }
}
