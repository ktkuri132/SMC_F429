#define __RELEASE
//#define __DEBUG

#if defined (__RELEASE)

#include <Project.h>

#include <RTOSTaskConfig.h>

/// @brief 主函数运行完了自动复位
void BSP_Init()
{
    xTaskCreate((TaskFunction_t)Task1_BSP_Init,
                                "Task1_BSP_Init",
                                128,
                                NULL,
                                10,
                                NULL);
    xTaskCreate((TaskFunction_t)Task2_Project_Init,
                                "Task2_Project_Init",
                                128,
                                NULL,
                                10,
                                NULL);
    vTaskStartScheduler();
    main();
}

void main()
{
    
}



#elif defined(__DEBUG)


#include <Project.h>

#include <inv_mpu.h>

float pitch,roll,yaw;

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

/// @brief 主函数运行完了自动复位
void BSP_Init()
{
Init_BSP:                                   // 初始化基本外设
    SDRAM_Init();                       
    OLED_Init();                        
    mpu_dmp_init();
    bsp_usart_1_inti(115200);
    bsp_usart_2_inti(9600);                     // 蓝牙串口
    bsp_usart_3_inti(115200);                   // OpenMV摄像头通信
    bsp_uart_4_inti(115200);                    // 无线串口
    bsp_uart_5_inti(115200);                    // V831摄像头通信
    // 检查OpenMV摄像头是否连接
    while (!(USART3->SR & USART_SR_RXNE))
    {
        usart_send_string(USART3,"你好");
    }
    // 检查V831摄像头是否连接
    while (!(UART5->SR & USART_SR_RXNE))
    {
        usart_send_string(UART5,"up");
    }
    // 检查无线串口（其他小车）是否在线
    while (!(UART4->SR & USART_SR_RXNE))
    {
        usart_send_string(UART4,"up");
    }
    
    
    
    NVIC_Configuration();           
             

Init_Serial:                                
    
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

Init_Project:
    Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    // Project_BSP_Encoding_Init();   printf("初始化编码器");
    // Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    // Project_BSP_HW201_Init();      printf("初始化红外传感器");
    // Project_LIB_TIM5_Init(5);
    while (1)
    {
        // usart_send_string(UART4,"hhh");
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        OLED_Printf(0,0,OLED_8X16,"OpenMV:%d",USART_Deal(&USART3_DataBuff,1));
        OLED_Printf(0,16,OLED_8X16,"V831:%d",USART_Deal(&UART5_DataBuff,1));
        OLED_Printf(0,32,OLED_8X16,"HFY:%d",USART_Deal(&UART4_DataBuff,1));
        OLED_Printf(0,48,OLED_8X16,"MPU6050:%f",pitch);
        OLED_Update();
    }
}


#endif