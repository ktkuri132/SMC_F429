// #define __RELEASE
#define __DEBUG
#include <Project/Dev/PWM/stm32f4xx_tim.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <BSP/sys.h>
#include <RTOS/comment/FreeRTOS.h>
#include <RTOS/comment/task.h>
#include <Project/Project.h>
#include <Project/Dev/HW_201/hw201.h>

#include "USER/RTOSTaskConfig.h"
#include "BSP/usart/Serial.h"
#include "BSP/usart/usart.h"
#include "HARDWARE/OLED/OLED.h"
#include "HARDWARE/MPU6050/inv_mpu.h"

#include <Project/LIB/Control/control.h>

// 定义串口数据结构
Stde_DataTypeDef USART2_DataBuff;
Stde_DataTypeDef USART3_DataBuff;
Stde_DataTypeDef UART5_DataBuff;
Stde_DataTypeDef UART4_DataBuff;

extern float pitch,roll,yaw;


void Test();
int main();
/// @brief 主函数运行完了自动复位
void BSP_Init()
{
Init_BSP:                                   // 初始化基本外设
    OLED_Init();                      
    mpu_dmp_init();
    bsp_usart_1_inti(115200);
    bsp_usart_2_inti(250000);                    // OpenMV摄像头通信 
    bsp_usart_3_inti(115200);                   // 数字识别摄像头
    bsp_uart_4_inti(115200);                    // 无线串口
    bsp_uart_5_inti(9600);                    // 蓝牙串口
    NVIC_Configuration();           

Init_Serial:                                
    Stde_DataTypeDef_Init(&USART2_DataBuff);
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

Init_Project:
    Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    Project_BSP_Encoding_Init();   printf("初始化编码器\n");
    Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    Project_BSP_HW201_Init();      printf("初始化红外传感器\n");
    Project_BSP_LED_Init();        printf("初始化LED\n");
    Project_BSP_Buzzer_Init();     printf("初始化蜂鸣器\n");
    Project_BSP_ADC_Init();        printf("初始化ADC\n");
    Project_BSP_KEY_Init();        printf("初始化按键\n");
    Project_LIB_TIM3_Init(10);     printf("初始化定时器\n");
    main();
}

// 创建任务句柄
TaskHandle_t *Task1_SystemStart_Handle;

TaskHandle_t *Task3_Project_Display_MPU6050_Handle;
TaskHandle_t *Task3_Project_Display_OpenMV_Handle;
TaskHandle_t *Task3_Project_Display_Voltage_Handle;
TaskHandle_t *Task3_Project_Display_Time_Handle;

TaskHandle_t *Task4_LEDPlayR_Handle;
TaskHandle_t *Task4_LEDPlayY_Handle;
TaskHandle_t *Task5_KeyScan_Handle;

int main()
{
    // xTaskCreate((TaskFunction_t)Task1_SystemStrat,"SystemStrat",1024,
    //                             1,10,Task1_SystemStart_Handle);
    // xTaskCreate((TaskFunction_t)Task3_Project_Display,"DisPlay_MPU6050",1024,
    //                             1,10,Task3_Project_Display_MPU6050_Handle);
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"DisPlay_Camer",1024,
                                2,10,Task3_Project_Display_OpenMV_Handle);
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"DisPlay_Voltage",1024,
                                3,10,Task3_Project_Display_Voltage_Handle);   
    // xTaskCreate((TaskFunction_t)Task3_Project_Display,"DisPlay_Time",1024,
    //                             4,10,Task3_Project_Display_Time_Handle);                                                      
    // xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Task4_LEDPlay",1024,
    //                             NULL,10,Task4_LEDPlay_Handle);
    xTaskCreate((TaskFunction_t)Task5_KeyScan,"Task5_KeyScan",1024,
                                NULL,10,Task5_KeyScan_Handle);
    vTaskStartScheduler();
    
}


void TIM3_IRQHandler()
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        Project_LIB_ControlStrat();
    }
}

void USART2_IRQHandler()
{
    if(USART2->SR & USART_SR_RXNE)
    {
        STDE_UART(USART2,&USART2_DataBuff);
    }
}

void USART3_IRQHandler()
{
    if(USART3->SR & USART_SR_RXNE)
    {
        STDE_UART(USART3,&USART3_DataBuff);
    }
}

void UART4_IRQHandler()
{
    if(UART4->SR & USART_SR_RXNE)
    {
        STDE_UART(UART4,&UART4_DataBuff);
    }
}

void UART5_IRQHandler()
{
    if(UART5->SR & USART_SR_RXNE)
    {
        STDE_UART(UART5,&UART5_DataBuff);
    }
}

#ifdef __cplusplus
}
#endif

