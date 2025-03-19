#include "Project.h"
#include "pid.h"
#include <Project/LIB/Control/control.h>

/*
 * 这里为什么要提供一个通用，还建议开发者提供专用的PID算法，通用的只有单极PID
 * 但是专用算法的可以随意发挥，并机，串级，这里提供了两个PID算法的接口，可供开发者提供最多两级的算法支持
 * 如有更多需求，可在结构体内添加，初始化函数中加入接口
 */
extern Stde_DataTypeDef USART2_DataBuff;
extern ctrl *Base;

void PID_TypeStructInit(PID *pid,int16_t kp,int16_t kd,int16_t ki,int16_t target)
{
    pid->max_output = MAX_PWM;
    pid->max_integral = 7200;
    pid->integral = 120;
    pid->target = target;
    pid->Kp = kp;
    pid->Kd = kd;
    pid->Ki = ki;
}


/// @brief PID 对于直线的专用控制函数
void PID_forLine(PID *pid)
{
    if((Base->Back_sign != 3)&&(Base->SiteLock != 4)){
        if(USART2_DataBuff.UART_DATA_TYPE != 1 ){
            return;
        }
    }
    
    pid->current = OpenMVData;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
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



void speedControl(PID *pid){

    (Base->Rvalue<0)?(Base->Rvalue=-Base->Rvalue):Base->Rvalue;
    (Base->Lvalue<0)?(Base->Lvalue=-Base->Lvalue):Base->Lvalue;

    pid->current = (Base->Rvalue+Base->Lvalue)/2;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
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

    ((Base->Rvalue)<0)?(Base->Rvalue=-Base->Rvalue):Base->Rvalue;
    ((Base->Lvalue)<0)?(Base->Lvalue=-Base->Lvalue):Base->Lvalue;

    pid->current = Base->Rvalue+Base->Lvalue;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
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


void BackControl(PID *pid)
{
    
    pid->current = (Motor->Left+Motor->Right)/2;

    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
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