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
#ifdef __cplusplus
extern "C"
{
#endif

#include <Project.h>
#include <RTOSTaskConfig.h>


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

TaskHandle_t *Task3_Project_Display_Mode_1_Handle;
TaskHandle_t *Task3_Project_Display_Mode_2_Handle;
TaskHandle_t *Task3_Project_Display_Mode_3_Handle;
TaskHandle_t *Task3_Project_Display_Mode_4_Handle;
TaskHandle_t *Task3_Project_Display_Mode_2_1_Handle;

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
                                2,10,Task3_Project_Display_Mode_2_Handle);
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"DisPlay_Voltage",1024,
                                3,10,Task3_Project_Display_Mode_3_Handle);   
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

#ifdef __cplusplus
}
#endif


#endif