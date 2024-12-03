#include <Project.h>

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;


/// @brief get the value of encoder on TIM4 and TIM5
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2)
{
	*value1 = TIM4->CNT;	
	*value2 = TIM5->CNT;
}


void Project_LIB_TIM5_Init(uint8_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1000*ms - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM5, DISABLE);
}

/// @brief Motor load function for left and right
void Project_LIB_Motor_Load(int32_t leftMotor,int32_t RightMotor)
{
    if(leftMotor<=0)
    {
        Motor->Left = -leftMotor;     
    }
    else 
    {
        Motor->Left = leftMotor;
    }
    if(RightMotor<=0)
    {
        Motor->Right = -RightMotor;
    }
    else 
    {
        Motor->Right = RightMotor;
    }
}


// @brief deal the data from the USART and these data will be from OpenMV or V831
uint8_t* Data_Save_from_Camer()
{
    static uint8_t Data;
    Data = V831Data;
    return &Data;
}