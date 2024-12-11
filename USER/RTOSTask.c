#include <Project.h>


extern Stde_DataTypeDef USART2_DataBuff,USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

void Task1_BSP_Init()
{
    
    bsp_usart_1_inti(115200);   // 产生调试信息
    bsp_usart_2_inti(9600);     // 蓝牙串口
    bsp_usart_3_inti(115200);   // OpenMV摄像头通信
    bsp_uart_4_inti(115200);    // 无线串口
    bsp_uart_5_inti(115200);    // V831摄像头通信
    OLED_Init();
    mpu_dmp_init();
    NVIC_Configuration();
}

void Task2_Project_Init()
{
    // 初始化串口数据结构
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

    // Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    // Project_BSP_Encoding_Init();   printf("初始化编码器");
    // Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    // Project_BSP_HW201_Init();      printf("初始化红外传感器");
}

float pitch,roll,yaw;


void Task3_Project_Display()
{
    while(1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        mpu_dmp_get_data(&pitch,&roll,&yaw);    
        OLED_Printf(0,0,OLED_6X8,"OpenMV:%d",USART_Deal(&USART2_DataBuff,1));
        // OLED_Printf(0,16,OLED_8X16,"V831:%d",USART_Deal(&UART5_DataBuff,1));
        OLED_Printf(0,8,OLED_6X8,"ADC:%.3f",Project_BSP_GetADC());
        // OLED_Printf(0,32,OLED_8X16,"HFY:%d",USART_Deal(&UART4_DataBuff,1));
        OLED_Printf(0,16,OLED_6X8,"HW201:%d",Project_BSP_HW201_Get());
        OLED_Printf(0,24,OLED_6X8,"MPU6050:%f",yaw);
        OLED_Update();

        if(!Project_BSP_HW201_Get())
        {
            Project_BSP_Buzzer_ON();
        }
        else
        {
            Project_BSP_Buzzer_OFF();
        }
        // 退出临界区
        taskEXIT_CRITICAL();
    }
}


void Task4_LEDPlay()
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 任务周期为 100 毫秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        Project_BSP_LED_ON(0);
        Project_BSP_LED_OFF(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        Project_BSP_LED_OFF(0);
        Project_BSP_LED_ON(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Task5_KeyScan()
{
    while(1)
    {
        
        

    }
}

