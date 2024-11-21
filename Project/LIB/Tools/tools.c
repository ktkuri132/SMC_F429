#include <Project.h>

/// @brief get the value of encoder on TIM4 and TIM5
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2)
{
	*value1 = TIM4->CNT;	
	*value2 = TIM5->CNT;
}