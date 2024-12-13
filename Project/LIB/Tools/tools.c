#include <Project.h>
#include <misc.h>

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;


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

/// @brief 将从串口读出的数据保存到Data中
/// @param Data 传入的数据
/// @return 1 读取到数据，0 未读取到数据
uint8_t Data_Save_from_Camer(uint8_t *Data)
{
    *Data = V831Data;
    if(*Data)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

/// @brief 严格的函数运行
/// @param pvfunction 待运行的函数
/// @param pvParameters 函数参数
void Task_run(void (*pvfunction)(void *),int16_t *pvParameters)
{
    uint8_t (*pvtempfunt)(void *) = pvfunction;
    while(!TimeOut(pvtempfunt((void *)(pvParameters)))){}
}

/// @brief 超时回调函数
/// @return 
uint8_t TimeOutCallBack()
{
    while (1)
    {
        //* 超时回调函数
    }
    
}



/// @brief 超时处理
/// @param returnData 传入1，返回1，传入0，返回0
/// @return 1 函数正常返回，0 执行超时
uint8_t TimeOut(uint8_t returnData)
{
    static uint8_t TimeOutCount = 0;
    if(returnData)
    {
        //* 传入数据合格
        TimeOutCount = 0;
        return 1;
    }
    else
    {
        TimeOutCount++;
        if(TimeOutCount > 100)
        {
            TimeOutCount = 0;
            TimeOutCallBack();
        }
        return 0;
    }
}

