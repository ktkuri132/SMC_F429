#include <Dev/PWM/stm32f4xx_tim.h>
#include <Dev/TB6612/tb6612_port.h>

#include <Project.h>
#include <misc.h>
#include <RTOSTaskConfig.h>
#include <usart/Serial.h>
#include <comment/task.h>
extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

extern TaskHandle_t *Task3_Project_Display_MPU6050_Handle;
extern TaskHandle_t *Task3_Project_Display_OpenMV_Handle;
extern TaskHandle_t *Task3_Project_Display_Voltage_Handle;
extern TaskHandle_t *Task3_Project_Display_Time_Handle;
extern TaskHandle_t *Task4_LEDPlayR_Handle;
extern TaskHandle_t *Task4_LEDPlayY_Handle;

/// @brief get the value of encoder on TIM4 and TIM5
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2)
{
	*value1 = TIM4->CNT;	
	*value2 = TIM5->CNT;
}



void Project_LIB_TIM3_Init(uint8_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1000*ms - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

uint8_t MotorStrat_1 = 1;   //  电机启动最高优先级：电源控制    默认插上跳线帽为标准功耗，开启电机，拔下跳线帽为低功耗，关闭电机
uint8_t MotorStrat_2 = 0;   //  电机启动第二优先级：药品放置    默认放置药品就开启电机，拿走药品就关闭电机
uint8_t MotorStrat_3 = 0;   //  电机启动第三优先级：巡线识别    默认巡线识别就开启电机，停止巡线识别就关闭电机

/// @brief Motor load function for left and right
void Project_LIB_Motor_Load(int32_t leftMotor,int32_t RightMotor)
{
    if(MotorStrat_1)
    {
        if(MotorStrat_2)
        {
            if(MotorStrat_3)
            {
                if(leftMotor<0)
                {
                    LeftBackward
                    Motor->Left = -leftMotor;     
                }
                else if(leftMotor>0)
                {
                    LeftForward
                    Motor->Left = leftMotor;
                }
                else
                {
                    Motor->Left = 0;
                }
                if(RightMotor<0)
                {
                    RightBackward
                    Motor->Right = -RightMotor;
                }
                else if(RightMotor>0)
                {
                    RightForward
                    Motor->Right = RightMotor;
                }
                else
                {
                    Motor->Right = 0;
                }
            }
            else
            {
                Motor->Left = 0;
                Motor->Right = 0;
                AllStop
            }
        }
        else
        {
            Motor->Left = 0;
            Motor->Right = 0;
            AllStop
        }
    }
    else
    {
        Motor->Left = 0;
        Motor->Right = 0;
        AllStop
    }
}


uint8_t CamerData[4];
/// @brief 将从串口读出的数据保存到Data中
/// @return -2 相等返回 -1 返回出现扫描空挡 0，1 返回当前存储的位置
uint8_t Data_Save_from_Camer()
{
    static uint8_t Temp;
    if(Temp==K210Data){
        return -2;    //  上次和这次数据一样
    }
    Temp = K210Data;    // 刷新临时数据
    static uint8_t i;
    if(Temp){   // 如果识别到了数字
        if(!CamerData[i]){     // 如果缓冲数组的目前为止为空
            CamerData[i] = Temp;        // 填入数字
            if(!i){     // 返回0闪红灯
                xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Red_LED",512,1,10,Task4_LEDPlayR_Handle);
            }
            else if(i){ // 返回1闪黄灯
                xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Yellow_LED",512,2,10,Task4_LEDPlayY_Handle);
            }
            i++;
            return i;
        }
    }
    else{   // 程序运行到这里，说明出现了扫描空挡
        return -1;   // 返回3只会出现在，第一次出现扫描空挡的瞬间
    }
}
