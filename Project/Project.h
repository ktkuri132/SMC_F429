#ifndef PROJECT_H
#define  PROJECT_H

// PWM duty limit(max)
#define MAX_PWM 9000

#ifdef __cplusplus
extern "C" 
{
#endif 

#include <stm32f4xx.h>
#include <gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <usart.h>
#include <OLED.h>

/*  project function in BSP folder    */
void Project_BSP_PWM_TIM2_Init();

/*    project funtion in Tools  folder    */
void Project_BSP_ENcoding_Init(uint16_t duty1,uint16_t duty2);


#ifdef __cplusplus
}
#endif // DEBUG




#endif // ! _