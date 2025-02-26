#include "Project.h"
#include "LIB/PID/pid.h"
#include "BSP/usart/Serial.h"
#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>

/*
 * 这里为什么要提供一个通用，还建议开发者提供专用的PID算法，通用的只有单极PID
 * 但是专用算法的可以随意发挥，并机，串级，这里提供了两个PID算法的接口，可供开发者提供最多两级的算法支持
 * 如有更多需求，可在结构体内添加，初始化函数中加入接口
 */

extern Stde_DataTypeDef USART2_DataBuff;

extern Stde_DataTypeDef USART3_DataBuff, UART5_DataBuff, UART4_DataBuff;

extern TaskHandle_t *Task3_Project_Display_Mode_1_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_2_1_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_3_Handle;
extern TaskHandle_t *Task3_Project_Display_Mode_4_Handle;
extern TaskHandle_t *Task4_LEDPlayR_Handle;
extern TaskHandle_t *Task4_LEDPlayY_Handle;

extern NCtrl nctrl;
extern MCtrl mctrl;
extern FCtrl fctrl;


void Dire_select_near(uint8_t Temp){
    static uint8_t Turn_const = 0;
    nctrl.old_RLControl = nctrl.RLControl;
    if (nctrl.SiteLock == 3)
    {
        if (Temp)
        {
            nctrl.RLControl = Temp;
        }
    }
    else if (nctrl.SiteLock == 1)
    {
        nctrl.RLControl = 0;
    }
    else
    {
        nctrl.RLControl = 0;
    }
    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (nctrl.old_RLControl != nctrl.RLControl)
    {
        Turn_const++;
        if (Turn_const == 2)
        { // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
            Turn_const    = 0;
            nctrl.Temp_RLContrl = 0;
        }
    }
}

void near_ctrlTask()
{
    nctrl.Data_Save_from_Camer();

    nctrl.Dire_select(nctrl.Temp_RLContrl);
    
    Project_LIB_ControlTask(nctrl.RLControl);
}

int8_t Data_Save_from_Camer_near()
{
    static uint8_t i = 0;
    if (!(nctrl.SaveDataLock))
    {
        // 刚开始没有数字等待数字靠近
        static uint8_t TempNum, TempSite;
        if (!K210Data)
        {
            return -1;
        }
        // 数字来了，判断是否和上次一样，一样就直接返回
        if (TempNum == K210Data)
        {
            return -2; //  上次和这次数据一样
        }

        TempNum = K210Data; // 和上次不一样，刷新临时数据
        if (!i)
        { // 返回0闪红灯
            xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Red_LED", 1024, 1, 10,
                        Task4_LEDPlayR_Handle);
        }
        i++;
        if (TempNum == 1)
        {
            nctrl.SaveDataLock = 1;
            nctrl.Temp_RLContrl = 1;
            return 1;
        }
        else if (TempNum == 2)
        {
            nctrl.SaveDataLock = 2;
            nctrl.Temp_RLContrl = 2;
            return 2;
        }
    }
    return -1;
}

void control_near_Init()
{
    nctrl.Data_Save_from_Camer = Data_Save_from_Camer_near;
    nctrl.ControlTask = near_ctrlTask;
    nctrl.Dire_select = Dire_select_near;

}


/// @brief PID 对于直线的专用控制函数
void PID_forLine(PID *pid)
{
    pid->current = OpenMVData;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
    pid->integral *= pid->Ki;
    // 积分限幅
    if (pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }
    else if (pid->integral < -pid->max_integral)
    {
        pid->integral = -pid->max_integral;
    }

    pid->derivative = pid->error - pid->last_error;

    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    // 输出限幅
    if (pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }
    else if (pid->output < -pid->max_output)
    {
        pid->output = -pid->max_output;
    }
    pid->last_error = pid->error;
}


extern int8_t Rvalue, Lvalue;


void speedControl(PID *pid){

    (Rvalue<0)?(Rvalue=-Rvalue):Rvalue;
    (Lvalue<0)?(Lvalue=-Lvalue):Lvalue;

    pid->current = (Rvalue+Lvalue)/2;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
    pid->integral *= pid->Ki;
    // 积分限幅
    if (pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }
    else if (pid->integral < -pid->max_integral)
    {
        pid->integral = -pid->max_integral;
    }

    pid->derivative = pid->error - pid->last_error;

    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    // 输出限幅
    if (pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }
    else if (pid->output < -pid->max_output)
    {
        pid->output = -pid->max_output;
    }
    pid->last_error = pid->error;
}

void TurnControl(PID *pid) {

    (Rvalue<0)?(Rvalue=-Rvalue):Rvalue;
    (Lvalue<0)?(Lvalue=-Lvalue):Lvalue;

    pid->current = Rvalue+Lvalue;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
    pid->integral *= pid->Ki;
    // 积分限幅
    if (pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }
    else if (pid->integral < -pid->max_integral)
    {
        pid->integral = -pid->max_integral;
    }

    pid->derivative = pid->error - pid->last_error;

    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    // 输出限幅
    if (pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }
    else if (pid->output < -pid->max_output)
    {
        pid->output = -pid->max_output;
    }
    (pid->output>0) ? (pid->output):(-pid->output);
    pid->last_error = pid->error;

}
