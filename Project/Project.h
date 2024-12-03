#ifndef PROJECT_H
#define  PROJECT_H



#ifdef __cplusplus
extern "C" 
{
#endif 

// C库函数
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 项目使用的芯片
#include <stm32f4xx.h>
// 开发板的SDK（官方库）
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
// 片上外设驱动
#include <gpio.h>
#include <usart.h>
#include <Serial.h>
// OS支持
#include <FreeRTOS.h>
#include <task.h>
// 外挂外设驱动
#include <OLED.h>
#include <inv_mpu.h>
// 项目头文件
#include <tb6612_port.h>
#include <pid.h>
#include <control.h>


// 定义电机接口寄存器
#define Motor TIM2
#define Left  CCR1
#define Right CCR2
// 定义PWM输出最大值
#define MAX_PWM 9000
// 定义从串口读出的数据
#define OpenMVData  USART_Deal(&USART3_DataBuff,1)
#define V831Data    USART_Deal(&UART5_DataBuff,1)
#define HFYCar      USART_Deal(&UART4_DataBuff,1)



/*  project function in BSP folder    */
void    Project_BSP_PWM_TIM2_Init();
void    Project_BSP_Encoding_Init();
void    Project_BSP_TB6612_Init();
void    Project_BSP_HW201_Init();
uint8_t Project_BSP_HW201_Get();

/*    project funtion in LIB folder    */
void Project_LIB_Get_Encoder_Value(uint16_t *value1,uint16_t *value2);
void Project_LIB_TIM5_Init(uint8_t ms);
void Project_LIB_Motor_Load(int32_t leftMotor,int32_t RightMotor);
uint8_t* Data_Save_from_Camer();
void Project_LIB_ControlTask();

#ifdef __cplusplus
}
#endif // DEBUG




#endif // ! _