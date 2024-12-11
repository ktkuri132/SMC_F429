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
    Project_LIB_TIM1_Init(5);      printf("初始化定时器\n");
    Project_LIB_Motor_Load(4500,4500);    printf("初始化电机\n");
    main();
}

// 创建任务句柄
TaskHandle_t *Task3_Project_Display_Handle;
TaskHandle_t *Task4_LEDPlay_Handle;
TaskHandle_t *Task5_KeyScan_Handle;

int main()
{
    xTaskCreate((TaskFunction_t)Task3_Project_Display,"Task1_BSP_Init",1024,
                                NULL,10,Task3_Project_Display_Handle);
    xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Task2_Project_Init",1024,
                                NULL,10,Task4_LEDPlay_Handle);
    
    vTaskStartScheduler();
}


void TIM1_UP_TIM10_IRQHandler()
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
        Project_LIB_ControlTask();
    }
}

void USART2_IRQHandler()
{
    if(USART2->SR & USART_SR_RXNE)
    {
        STDE_UART(USART2,&USART2_DataBuff);
    }
}

#ifdef __cplusplus
}
#endif


#endif