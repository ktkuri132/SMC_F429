// #define __RELEASE
#include "stm32f4xx.h"
#include <Dev/PWM/stm32f4xx_tim.h>
#include <stdio.h>
#include <LIB/PID/pid.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <Dev/HW_201/hw201.h>
#include <LIB/Control/control.h>
#include <Project.h>
#include <W25Q64/spiFlash.h>
#include <comment/FreeRTOS.h>
#include <comment/task.h>
#include <sys.h>
#include "main.h"

#include "MPU6050/inv_mpu.h"
#include "OLED/OLED.h"
#include "RTOSTaskConfig.h"
#include "usart/Serial.h"
#include "usart/usart.h"

#include <Project.h>

// 定义串口数据结构
Stde_DataTypeDef USART2_DataBuff;
Stde_DataTypeDef USART3_DataBuff;
Stde_DataTypeDef UART5_DataBuff;
Stde_DataTypeDef UART4_DataBuff;

// 定义控制结构体指针
ctrl *Base;
nctrl *Near;
mctrl *Min;
fctrl *Far;
PET   *Pet;

/// @brief 主函数运行完了自动复位
void BSP_Init() {
Init_BSP:  // 初始化基本外设
    OLED_Init();
    mpu_dmp_init();
    bsp_usart_1_inti(115200);
    bsp_usart_2_inti(250000);  // OpenMV摄像头通信
    bsp_usart_3_inti(115200);  // 数字识别摄像头
    bsp_uart_4_inti(115200);   // 无线串口
    bsp_uart_5_inti(9600);     // 蓝牙串口
    NVIC_Configuration();

Init_Serial:
    Stde_DataTypeDef_Init(&USART2_DataBuff);
    USART2_DataBuff.UART_DATA_TYPE_Callback = OpenMV_Camera_Callback;
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

Init_Control:       
    Base = Control_Struct_Inti();   // 初始化控制结构体
    
Init_Project:
    Project_BSP_PWM_TIM2_Init();
    Project_BSP_Encoding_Init();
    Project_BSP_TB6612_Init();
    Project_BSP_HW201_Init();
    Project_BSP_LED_Init();
    Project_BSP_Buzzer_Init();
    Project_BSP_ADC_Init();
    Project_BSP_KEY_Init();
    Project_LIB_TIM3_Init(5);
    main();
    // Test();
}

// 创建任务句柄
TaskHandle_t Task1_SystemStart_Handle = NULL;
TaskHandle_t Task_DebugLog_Handle = NULL;

int main() {
    if (xTaskCreate((TaskFunction_t)Task_DebugLog, "DebugLog", 1024, NULL, 9,
                    &Task_DebugLog_Handle) != pdPASS) {
        printf("HeapManager创建失败\n");
    }
    if (xTaskCreate((TaskFunction_t)Task1_SystemStrat, "SystemStrat", 1024,
                    NULL, 9, &Task1_SystemStart_Handle) != pdPASS) {
        printf("SystemStrat创建失败\n");
    }

    // 初始化控制结构体

    vTaskStartScheduler();
}



void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        Project_LIB_Get_Encoder_Value(&(Base->Rvalue), &(Base->Lvalue));
        Base->ControlTask();
    }
}

void USART2_IRQHandler() {
    if (USART2->SR & USART_SR_RXNE) {
        STDE_UART(USART2, &USART2_DataBuff);
    }
}

void USART3_IRQHandler() {
    if (USART3->SR & USART_SR_RXNE) {
        STDE_UART(USART3, &USART3_DataBuff);
    }
}

void UART4_IRQHandler() {
    if (UART4->SR & USART_SR_RXNE) {
        STDE_UART(UART4, &UART4_DataBuff);
    }
}

void UART5_IRQHandler() {
    if (UART5->SR & USART_SR_RXNE) {
        STDE_UART(UART5, &UART5_DataBuff);
    }
}

#ifdef __cplusplus
}
#endif
