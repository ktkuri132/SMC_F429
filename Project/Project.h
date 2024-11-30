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
#include <Serial.h>
#include <OLED.h>
#include <tb6612_port.h>
#include <inv_mpu.h>

#define Motor TIM2
#define Left  CCR1
#define Right CCR2
/*  project function in BSP folder    */
void Project_BSP_PWM_TIM2_Init();
void Project_BSP_Encoding_Init();
void Project_BSP_TB6612_Init();
void Project_BSP_HW201_Init();
uint8_t Project_BSP_HW201_Get();

/*    project funtion in LIB folder    */
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2);
void Project_LIB_TIM5_Init(uint8_t ms);
void Project_LIB_Motor_Load(int32_t leftMotor,int32_t RightMotor);

#ifdef __cplusplus
}
#endif // DEBUG




#endif // ! _