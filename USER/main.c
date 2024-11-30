// #define __RELEASE
#define __DEBUG

#if defined (__RELEASE)

#include <Project.h>

#include <RTOSTaskConfig.h>


TaskHandle_t *Task1_BSP_Init_Handle;
TaskHandle_t *Task2_Project_Init_Handle;
TaskHandle_t *Task3_Project_Display_Handle;
/// @brief 主函数运行完了自动复位
void BSP_Init()
{
    xTaskCreate((TaskFunction_t)Task1_BSP_Init,"Task1_BSP_Init",128,
                                NULL,10,Task1_BSP_Init_Handle);
    xTaskCreate((TaskFunction_t)Task2_Project_Init,"Task2_Project_Init",128,
                                NULL,10,Task2_Project_Init_Handle);
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"Task3_Project_Display",1024,
                                NULL,9,Task3_Project_Display_Handle);
    vTaskStartScheduler();
    main();

}

void main()
{
    while (1)
    {
        
    }
    
}



#elif defined(__DEBUG)


#include <Project.h>
#include <RTOSTaskConfig.h>


extern float pitch,roll,yaw;

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

TaskHandle_t *Task1_BSP_Init_Handle;
TaskHandle_t *Task2_Project_Init_Handle;
TaskHandle_t *Task3_Project_Display_Handle;

void Test();

/// @brief 主函数运行完了自动复位
void BSP_Init()
{
Init_BSP:                                   // 初始化基本外设
    // SDRAM_Init(); 
    OLED_Init();                      
    mpu_dmp_init();
    bsp_usart_1_inti(115200);
    bsp_usart_2_inti(9600);                     // 蓝牙串口
    bsp_usart_3_inti(115200);                   // OpenMV摄像头通信
    bsp_uart_4_inti(115200);                    // 无线串口
    bsp_uart_5_inti(115200);                    // V831摄像头通信
    NVIC_Configuration();           
             

Init_Serial:                                
    
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

Init_Project:
    Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    Project_BSP_Encoding_Init();   printf("初始化编码器");
    Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    Project_BSP_HW201_Init();      printf("初始化红外传感器");
    Project_LIB_TIM5_Init(5);
    main();
    

 
}

void main()
{
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"Task3_Project_Display",1024,
                                NULL,32,Task3_Project_Display_Handle);
    
    vTaskStartScheduler();
}


void Test()
{
   
    
}


#endif