#include <Project.h>
#include <RTOSTaskConfig.h>
// 设置任务栈
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__((section(".ext_sram")));

extern Stde_DataTypeDef USART2_DataBuff,USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

extern TaskHandle_t *Task3_Project_Display_Mode_1_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_3_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_4_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_1_Handle;
extern TaskHandle_t *Task4_LEDPlayR_Handle;
extern TaskHandle_t *Task4_LEDPlayY_Handle;

extern uint8_t CamerData[4];

/// @brief 系统启动线程：协助完成模式选择，图象识别
void Task1_SystemStrat(uint8_t Mode)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency_5 = pdMS_TO_TICKS(5); // 任务周期为 5 毫秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    static uint8_t Image_identify_Return;
    switch (Mode)
    {
    case 1:
        goto Image_identify;
        break;
    default:
        break;
    }
Image_identify:
    do{
        Image_identify_Return = Data_Save_from_Camer();     // 刷新数据存储
        if(!Image_identify_Return){     // 返回0闪红灯
            xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Red_LED",512,1,10,Task4_LEDPlayR_Handle);
        }
        else if(Image_identify_Return){ // 返回1闪黄灯
            xTaskCreate((TaskFunction_t)Task4_LEDPlay,"Yellow_LED",512,2,10,Task4_LEDPlayY_Handle);
        }
        xTaskDelayUntil(&xLastWakeTime,xFrequency_5);   // 释放线程，每5ms刷新一次线程
    }while ((Image_identify_Return == -2)||(Image_identify_Return == -1));
    return Image_identify_Return;
}

void Task2_Project_Init()
{
    
}

float pitch,roll,yaw;
short gyro[3];
extern uint8_t MotorStrat_1,MotorStrat_2,MotorStrat_3;

uint8_t TaskDeletSign = 0;      // 函数删除信号

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
            case 21:
            {
                goto Mode_2_1;
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
        if(TaskDeletSign == 1){
            goto Mode_1_Out;
        }
    }
Mode_1_Out:
    vTaskDelete(Task3_Project_Display_Mode_1_Handle);
    TaskDeletSign = 0;  // 回应删除信号
    return 0;
Mode_2:     // 加入监测OpenMV
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();   
        // 检查任务是否删除，放前面给予任务最后一次运行机会，放后面不运行最后一次
        if(TaskDeletSign == 2){
            vTaskDelete(Task3_Project_Display_Mode_2_Handle);
            TaskDeletSign = 0;  // 回应删除信号
        }
        OLED_Printf(0,8,OLED_6X8,"OpenMV:%d",OpenMVData);
        OLED_Printf(0,16,OLED_6X8,"K210:%d,%d",CamerData[0],CamerData[1]);
        OLED_Update();
        if(USART_Deal(&USART2_DataBuff,0)==1)
        {
            MotorStrat_3 = 1;
        }
        else
        {
            MotorStrat_3 = 0;
        }
        
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
        
    }
Mode_2_1:
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0,8,OLED_6X8,"OpenMV:%d",OpenMVData);
        OLED_Printf(0,32,OLED_6X8,"K210:%d,%d",K210Data,CamerData[3]);
        OLED_Update();
        if(USART_Deal(&USART2_DataBuff,0)==1)
        {
            MotorStrat_3 = 1;
        }
        else
        {
            MotorStrat_3 = 0;
        }
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
        if(TaskDeletSign == 21){
            goto Mode_2_1_Out;
        }
    }
Mode_2_1_Out:
    vTaskDelete(Task3_Project_Display_Mode_2_1_Handle);
    TaskDeletSign = 0;  // 回应删除信号
    return 0;
Mode_3:     // 加入监测电池电压
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_DrawRectangle(100,0,28,16,OLED_UNFILLED);
        OLED_Printf(104,4,OLED_6X8,"%.0f",Project_BSP_GetADC());
        OLED_ShowChar(116,4,'%',OLED_6X8);
        if(!Project_BSP_GetADC())
        {
            MotorStrat_1 = 0;       // 电池当前电量为0
        }
        else
        {
            MotorStrat_1 = 1;
        }
        // OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_100);
        if(TaskDeletSign == 3){
            goto Mode_3_Out;
        }
    }
Mode_3_Out:
    vTaskDelete(Task3_Project_Display_Mode_3_Handle);
    TaskDeletSign = 0;  // 回应删除信号
    return 0;
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
        if(TaskDeletSign == 4){
            goto Mode_4_Out;
        }
    }
Mode_4_Out:
    vTaskDelete(Task3_Project_Display_Mode_4_Handle);
    TaskDeletSign = 0;  // 回应删除信号
    return 0;
}


void Task4_LEDPlay(uint8_t Mode)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 任务周期为 100 毫秒
    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    uint8_t RunCounst = 2;
    switch (Mode)
    {
    case 1:
        goto Red_LED;
        break;
    case 2:
        goto Yellow_LED;
        break;
    default:
        break;
    }
Red_LED:
    while(RunCounst)
    {
        Project_BSP_LED_ON(0);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        Project_BSP_LED_OFF(0);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        RunCounst--;
    }
    vTaskDelete(Task4_LEDPlayR_Handle);
    return 0;
Yellow_LED:
    while (RunCounst)
    {
        Project_BSP_LED_ON(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        Project_BSP_LED_OFF(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        RunCounst--;
    }
    vTaskDelete(Task4_LEDPlayY_Handle);
    return 0;
}




void Task5_KeyScan()
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // 任务周期为 100 毫秒

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

        if(!Project_BSP_HW201_Get())
        {
            OLED_Printf(0,64,OLED_6X8,"HW201:1");
            MotorStrat_2 = 1;
        }
        else
        {
            OLED_Printf(0,64,OLED_6X8,"HW201:0");
            MotorStrat_2 = 0;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

    }
}








