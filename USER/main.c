//#define __RELEASE
#define __DEBUG

#if defined (__RELEASE)

#include <Project.h>


/// @brief 主函数运行完了自动复位
void BSP_Init()
{
    SDRAM_Init();
    bsp_usart_1_inti(115200);
    

}



#elif defined(__DEBUG)


#include <Project.h>

#include <inv_mpu.h>

float pitch,roll,yaw;
Stde_DataTypeDef USART3_DataBuff;
Stde_DataTypeDef UART5_DataBuff;
/// @brief 主函数运行完了自动复位
void BSP_Init()
{
Init_BSP:                                   // 初始化基本外设
    SDRAM_Init();                       
    OLED_Init();                        
    mpu_dmp_init();
    bsp_usart_1_inti(115200);               // 调试信息       
    // bsp_usart_2_inti(9600);                 // 蓝牙串口
    bsp_usart_3_inti(115200);                // OpenMV摄像头通信
    // bsp_uart_4_inti(115200);                // 无线串口
    bsp_uart_5_inti(115200);                // V831摄像头通信
    NVIC_Configuration();           
             

Init_Serial:                                
    
    // Stde_DataTypeDef_Init(&USART3_DataBuff);
    // Stde_DataTypeDef_Init(&UART5_DataBuff);

Init_Project:
    // Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    // Project_BSP_Encoding_Init();   printf("初始化编码器");
    // Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    // Project_BSP_HW201_Init();      printf("初始化红外传感器");
    while (1)
    {
        // usart_send_string(USART3,"hhh");
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        // OLED_Printf(0,0,OLED_8X16,"%d",USART_Deal(&USART3_DataBuff,1));
        OLED_Printf(0,48,OLED_8X16,"%f",pitch);
        OLED_Update();
    }
    
   
    
}


void USART1_IRQHandler()
{
    if(USART1->SR & USART_SR_RXNE)
    {
        OLED_Printf(0,16,OLED_8X16,"%c",USART1->DR);
        OLED_Update();
    }
}



void USART3_IQRHandler()
{
    if(USART3->SR & USART_SR_RXNE)
    {
        
        OLED_Printf(0,16,OLED_8X16,"%c",USART3->DR);
        OLED_Update();
        // STDE_UART(USART3,&USART3_DataBuff);
    }
}


void UART5_IRQHandler()
{
    if(UART5->SR & USART_SR_RXNE)
    {
        OLED_Printf(0,16,OLED_8X16,"%c",UART5->DR);
        OLED_Update();
    }
}

#endif