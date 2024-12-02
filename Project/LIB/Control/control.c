#include <Project.h>
#include <control.h>

// base PID control algorithm
void PID_forLine(PID *pid)
{
    pid->error = pid->target - pid->current;
    pid->integral += pid->error;
    pid->integral *= pid->Ki;
    // 积分限幅
    if(pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }
    else if(pid->integral < -pid->max_integral)
    {
        pid->integral = -pid->max_integral;
    }
    pid->derivative = pid->error - pid->last_error;
    pid->output = pid->Kp * pid->error + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    // 输出限幅
    if(pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }
    else if(pid->output < -pid->max_output)
    {
        pid->output = -pid->max_output;
    }
    pid->last_error = pid->error;
}

void PID_forTurn(PID *pid)
{
    
}

void PID_StartRun(PID *pid)
{

}

void PID_TypeStructInit(PID *pid,void *lineControl,int16_t linecurrent,void *turnControl,int16_t turnparam)
{
    pid->max_output = MAX_PWM;
    pid->max_integral = 7200;
    pid->integral = 120;
    pid->current = current;
    pid->PID_forLine = lineControl;
    pid->PID_forTurn = turnControl;
}
