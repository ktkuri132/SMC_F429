#include <Project.h>

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;


/// @brief get the value of encoder on TIM4 and TIM5
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2)
{
	*value1 = TIM4->CNT;	
	*value2 = TIM5->CNT;
}


void Project_LIB_TIM1_Init(uint8_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1000*ms - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}

/// @brief Motor load function for left and right
void Project_LIB_Motor_Load(int32_t leftMotor,int32_t RightMotor)
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

