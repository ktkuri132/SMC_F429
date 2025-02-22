#include "Project.h"
#include "LIB/PID/pid.h"
#include "BSP/usart/Serial.h"

/*
 * 这里为什么要提供一个通用，还建议开发者提供专用的PID算法，通用的只有单极PID
 * 但是专用算法的可以随意发挥，并机，串级，这里提供了两个PID算法的接口，可供开发者提供最多两级的算法支持
 * 如有更多需求，可在结构体内添加，初始化函数中加入接口
 */

extern Stde_DataTypeDef USART2_DataBuff;

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

/// @brief PID 对于转向的专用控制函数
void PID_forTurn(PID *pid)
{

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
