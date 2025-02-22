#ifndef PROJECT_H
#define PROJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

// C库函数
#include <stdio.h>
#include <string.h>
// 项目使用的芯片
// 开发板的SDK（官方库）
// #include "stm32f4xx_rcc.h"
// 片上外设驱动
// OS支持
#include <RTOS/comment/FreeRTOS.h>

// 外挂外设驱动
#include <HARDWARE/OLED/OLED.h>
#include <usart/Serial.h>
// 项目头文件

// 定义电机接口寄存器
#define Motor TIM2
#define Left CCR1
#define Right CCR2
// 定义PWM输出最大值
#define MAX_PWM 9000
// 定义从串口读出的数据
#define OpenMVData USART_Deal(&USART2_DataBuff, 1)
#define K210Data USART_Deal(&USART3_DataBuff, 1)
#define K210Site USART_Deal(&USART3_DataBuff, 2)
#define HFYCar USART_Deal(&UART4_DataBuff, 1)

    /*  project function in BSP folder    */
    void Project_BSP_PWM_TIM2_Init();
    void Project_BSP_Encoding_Init();
    void Project_BSP_TB6612_Init();
    // void    Project_BSP_HW201_Init();
    // uint8_t Project_BSP_HW201_Get();

    /*    project funtion in LIB folder    */
    void Project_LIB_Get_Encoder_Value(int8_t *value1, int8_t *value2);
    void Project_LIB_TIM3_Init(uint8_t ms);
    void Project_LIB_TIM1_Init(uint8_t ms);
    void Project_LIB_Motor_Load(int32_t leftMotor, int32_t RightMotor);
    int8_t Data_Save_from_Camer();
    void Project_LIB_ControlTask();

    // ADC
    void Project_BSP_ADC_Init();
    float Project_BSP_GetADC();

    // LED
    void Project_BSP_LED_Init();
    void Project_BSP_LED_ON(uint8_t LED);
    void Project_BSP_LED_OFF(uint8_t LED);
    void Project_BSP_Buzzer_Init();
    void Project_BSP_Buzzer_ON();
    void Project_BSP_Buzzer_OFF();

    // KEY
    void Project_BSP_KEY_Init();
    uint8_t Project_BSP_GetKey();

    // Tools
    void OpenMV_Camera_Callback(Stde_DataTypeDef *DataTypeStruct);
    uint8_t Temp_Run(void *(Fp)());
    int8_t Data_Get_from_Camer();

#ifdef __cplusplus
}
#endif // DEBUG

#endif // ! _