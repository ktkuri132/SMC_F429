#include <Project.h>

void Project_BSP_GET_ENCODER_VALUE(uint16_t *value1,uint16_t *value2)
{
	*value1 = TIM4->CNT;	
	*value2 = TIM5->CNT;
}