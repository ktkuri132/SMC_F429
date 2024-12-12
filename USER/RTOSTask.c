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
short gyro[3];

/// @brief 显示输出
/// @param Mode 显示模式
void Task3_Project_Display(uint8_t Mode)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency_5 = pdMS_TO_TICKS(5); // 任务周期为 5 毫秒
    const TickType_t xFrequency_100 = pdMS_TO_TICKS(100); // 任务周期为 100 毫秒
    const TickType_t xFrequency_1000 = pdMS_TO_TICKS(2000); // 任务周期为 2秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    switch (Mode)
    {
        case 1:
        {
            goto Mode_1;
        }
        break;
        case 2:
        {
            goto Mode_2;
        }break;
        case 3:
        {
            goto Mode_3;
        }break;
        case 4:
        {
            goto Mode_4;
        }break;
    default:
        break;
    }

Mode_1:     // 加入监测MPU6050
    while(1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        mpu_dmp_get_data(&pitch,&roll,&yaw);    
        OLED_Printf(0,24,OLED_6X8,"MPU6050:%f",yaw);
        OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
    }
Mode_2:     // 加入监测OpenMV
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0,8,OLED_6X8,"OpenMV:%d",USART_Deal(&USART2_DataBuff,1));
        OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
    }
Mode_3:     // 加入监测电池电压
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0,0,OLED_6X8,"Voltage:%.1f",Project_BSP_GetADC());
        // OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_100);
    }
Mode_4:
    static hour,min,sec;
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0,16,OLED_6X8,"time:%d:%d:%d",hour,min,sec);
        if(sec==60)
        {
            sec=0;
            min++;
            if(min==60)
            {
                min=0;
                hour++;
                if(hour==24)
                {
                    hour=0;
                }
            }
        }
        else
        {
            sec++;
        }
        // OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_1000);
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


extern TaskHandle_t *Task3_Project_Display_MPU6050_Handle;
extern TaskHandle_t *Task3_Project_Display_OpenMV_Handle;
extern TaskHandle_t *Task3_Project_Display_Voltage_Handle;
extern TaskHandle_t *Task3_Project_Display_Time_Handle;

void Task5_KeyScan()
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 任务周期为 100 毫秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    static TaskStrat[3];
    while(1)
    {
        uint8_t temp = Project_BSP_GetKey();
        
        if(temp)
        {
            Project_BSP_Buzzer_ON();
        }
        else
        {
            Project_BSP_Buzzer_OFF();
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

    }
}

