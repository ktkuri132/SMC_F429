#include "LIB/Control/control.h"
#include <BSP/usart/Serial.h>
#include <HARDWARE/MPU6050/inv_mpu.h>
#include <OLED/OLED.h>
#include <Project/Dev/HW_201/hw201.h>
#include <Project/Project.h>
#include <RTOS/comment/task.h>
#include <RTOSTaskConfig.h>
#include <stdint.h>
#include <stdio.h>

// 设置任务栈
uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section(".ccmram")));

extern Stde_DataTypeDef USART2_DataBuff, USART3_DataBuff, UART5_DataBuff, UART4_DataBuff;

extern TaskHandle_t Task1_SystemStart_Handle;
extern TaskHandle_t Task_DebugLog_Handle;
TaskHandle_t Task3_Project_Display_Mode_1_Handle   = NULL;
TaskHandle_t Task3_Project_Display_Mode_2_Handle   = NULL;
TaskHandle_t Task3_Project_Display_Mode_2_1_Handle = NULL;
TaskHandle_t Task3_Project_Display_Mode_3_Handle   = NULL;
TaskHandle_t Task3_Project_Display_Mode_4_Handle   = NULL;
TaskHandle_t Task4_LEDPlayR_Handle = NULL;
TaskHandle_t Task4_LEDPlayY_Handle = NULL;
TaskHandle_t Task5_KeyScan_Handle  = NULL;
extern ctrl *Base;
extern nctrl *Near;
extern mctrl *Min;
extern fctrl *Far;
extern PET *Pet;

/// @brief 系统启动主线程：完成模式选择，图象识别
void Task1_SystemStrat()
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 任务周期为 5 毫秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    // 显示摄像头图像接受
    if (xTaskCreate((TaskFunction_t)Task3_Project_Display, "DisPlay_Camer", 1024, 2, 9,
                    &Task3_Project_Display_Mode_2_Handle) != pdPASS)
    {
        printf("DisPlay_Camer创建失败\n");
    }
    // // 系统显示时间
    // if (xTaskCreate((TaskFunction_t)Task3_Project_Display, "DisPlay_Time", 1024, 4, 9,
    //                 &Task3_Project_Display_Mode_4_Handle) != pdPASS)
    // {
    //     printf("DisPlay_Time创建失败\n");
    // }
    // 按键扫描
    if (xTaskCreate((TaskFunction_t)Task5_KeyScan, "Task5_KeyScan", 1024, NULL, 9,
                    &Task5_KeyScan_Handle) != pdPASS)
    {
        printf("Task5_KeyScan创建失败\n");
    }
    // 显示当前电压
    if (xTaskCreate((TaskFunction_t)Task3_Project_Display, "DisPlay_Voltage", 1024, 3, 9,
                    Task3_Project_Display_Mode_3_Handle) != pdPASS)
    {
        printf("DisPlay_Voltage创建失败\n");
    }
    // 按键配置:1->确定 2->向下选择 3->返回
    while (1)
    {
        if (Base->Key_Value) {
            taskENTER_CRITICAL();
            // 数字收集完成,删除对应显示程序
            vTaskDelete(Task3_Project_Display_Mode_2_Handle);
            Task3_Project_Display_Mode_2_Handle = NULL;
            // 刷新OLED
            OLED_Clear();
            OLED_Update();
            // 创建读取显示任务
            if (xTaskCreate((TaskFunction_t)Task3_Project_Display, "DisPlay_Camer", 1024, 21, 9,
                            Task3_Project_Display_Mode_2_Handle) != pdPASS)
            {
                printf("DisPlay_Camer创建失败\n");
            }
            taskEXIT_CRITICAL();
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/// @brief 转向状态检测
void STEERING_STATUS_DETECTION()
{
    TickType_t xLastWakeTime;
    
}

float pitch, roll, yaw;
short gyro[3];

/// @brief 显示输出
/// @param Mode 显示模式
void Task3_Project_Display(uint8_t Mode)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency_5    = pdMS_TO_TICKS(5);    // 任务周期为 5 毫秒
    const TickType_t xFrequency_100  = pdMS_TO_TICKS(100);  // 任务周期为 100 毫秒
    const TickType_t xFrequency_1000 = pdMS_TO_TICKS(2000); // 任务周期为 2秒
    static int hour, min, sec;

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    switch (Mode)
    {
    case 1: {
        goto Mode_1;
    }
    break;
    case 2: {
        goto Mode_2;
    }
    break;
    case 21: {
        goto Mode_21;
    }
    break;
    case 3: {
        goto Mode_3;
    }
    break;
    case 4: {
        goto Mode_4;
    }
    break;
    default:
        break;
    }

Mode_1: // 加入监测MPU6050
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        mpu_dmp_get_data(&pitch, &roll, &yaw);
        OLED_Printf(0, 24, OLED_6X8, "MPU6050:%f", yaw);
        OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
    }
Mode_2: // 加入监测摄像头
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0, 8, OLED_6X8, "K210 Get:%d",Base->CamerData[0]);
        OLED_Update();
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
    }
Mode_21:
    while (1)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        OLED_Printf(0, 0, OLED_6X8, "OpenMV:%d", OpenMVData);
        OLED_Printf(0, 8, OLED_6X8, "K210:  %d,%d",Base->CamerVerify[0],Base->CamerVerify[1]);
        OLED_Printf(0, 16, OLED_6X8, "Path: %d",Base->j);
        OLED_Printf(0, 24, OLED_6X8, "RL:   %d", Base->RLControl);
        OLED_Printf(0, 32, OLED_6X8, "TRL:  %d", Base->Temp_RLContrl);
        OLED_Printf(0, 40, OLED_6X8, "Mode: %d", Base->Key_Value);
        OLED_Update();

        if (USART2_DataBuff.UART_DATA_TYPE != 2)
        {
            Base->MotorStrat_3 = 1;
        }
        else
        {
            Base->MotorStrat_3 = Base->MotorStrat_3_POINT;
        }
        // 退出临界区
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_5);
    }
Mode_3: // 加入监测电池电压
    while (1)
    {
        // 进入临界区
        // taskENTER_CRITICAL();
        OLED_DrawRectangle(100, 0, 28, 16, OLED_UNFILLED);
        OLED_Printf(104, 4, OLED_6X8, "%.0f", Project_BSP_GetADC());
        OLED_ShowChar(116, 4, '%', OLED_6X8);
        if (!Project_BSP_GetADC())
        {
            Base->MotorStrat_1 = 0; // 电池当前电量为0
        }
        else
        {
            Base->MotorStrat_1 = 1;
        }
        // OLED_Update();
        // 退出临界区
        // taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, xFrequency_100);
    }
Mode_4:
    while (1)
    {
        // 进入临界区
        // taskENTER_CRITICAL();
        OLED_Printf(0, 0, OLED_6X8, "time:%d:%d:%d", hour, min, sec);
        if (sec == 60)
        {
            sec = 0;
            min++;
            if (min == 60)
            {
                min = 0;
                hour++;
                if (hour == 24)
                {
                    hour = 0;
                }
            }
        }
        else
        {
            sec++;
        }
        
        vTaskDelayUntil(&xLastWakeTime, xFrequency_1000);
    }
}

void Task4_LEDPlay(uint8_t Mode)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 任务周期为 100 毫秒
    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime     = xTaskGetTickCount();
    uint8_t RunCounst = 1;
    switch (Mode)
    {
    case 1:
        goto Red_LED;
        break;
    case 2:
        goto Yellow_LED;
    default:
        break;
    }
Red_LED:
    while (RunCounst)
    {
        Project_BSP_LED_ON(0);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        Project_BSP_LED_OFF(0);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        RunCounst--;
    }
    vTaskDelete(NULL);
Yellow_LED:
    while (RunCounst)
    {
        Project_BSP_LED_ON(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        Project_BSP_LED_OFF(1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        RunCounst--;
    }
    vTaskDelete(NULL);
}


void Task_DebugLog()
{
    TickType_t xLastWakeTime;
    extern Stde_DataTypeDef UART4_DataBuff;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 任务周期为 1秒
    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        taskENTER_CRITICAL();
        printf(CLEAR_SCREEN);
        printf(CURSOR_HOME);
        printf(CURSOR_HIDE);
        // printf("剩余栈大小:%d\n", xPortGetFreeHeapSize());
        // printf("任务数量:%d\n", uxTaskGetNumberOfTasks());
        printf("SiteLock:           %d\n", Base->SiteLock);
        printf("经过弯道次数:        %d\n", Base->j);
        printf("RLControl:          %d\n", Base->RLControl);
        printf("Temp_RLContrl:      %d\n", Base->Temp_RLContrl);
        printf("Back_sign:          %d\n", Base->Back_sign);
        printf("VerifyDataLock:     %d\n", Base->VerifyDataLock);
        printf("CamerData[0]:       %d\n", Base->CamerData[0]);
        printf("验证数据及预定方向:  %d,%d\n", Base->CamerVerify[0], Base->CamerVerify[1]);
        printf("运行模式:           %d\n", Base->Key_Value);
        printf("-----------------------------------------------------------\n");
        if(Base->Key_Value == 1){

        } else if(Base->Key_Value == 2) {

        } else if(Base->Key_Value == 3) {
            
        } else if(Base->Key_Value == 4) {
            printf("错误点:     %d\n",Pet->Error);
            printf("运行状态:   %d\n",Pet->Runstate);
            printf("运行状态2:  %d\n",Pet->Runstate_2);
            printf("临时状态:   %d\n",Pet->temp);
            printf("检测路口:   %d\n",Pet->PathNum);
            printf("数据锁:     %d\n",Pet->SDL);
            printf("错误信息:   %s\n",Pet->ErrorChar[Pet->Error]);
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        taskEXIT_CRITICAL();
    }
}

void Task5_KeyScan()
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // 任务周期为 100 毫秒

    // 初始化 xLastWakeTime 变量为当前时间
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        if (Project_BSP_GetKey()) {
            Base->Key_Value = Project_BSP_GetKey();
        }

        if (Project_BSP_GetKey())
        {
            Project_BSP_Buzzer_ON();
        }
        else
        {
            Project_BSP_Buzzer_OFF();
        }

        if (!Project_BSP_HW201_Get())
        {
            OLED_Printf(0, 48, OLED_6X8, "HW201:1");
            Base->MotorStrat_2 = 1;
        }
        else
        {
            OLED_Printf(0, 48, OLED_6X8, "HW201:0");
            Base->MotorStrat_2 = 0;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
