#if !defined(__PID__)
#define __PID__


#include "Project.h"
#include "pid.h"




typedef struct PID_TypeDef
{
    /* function  */
    void (*PID_Update1)(struct PID_TypeDef *pid);
    void (*PID_Update2)(struct PID_TypeDef *pid);
    void (*PID_Update3)(struct PID_TypeDef *pid);
    /* data */
    int32_t error;
    int32_t last_error;
    int32_t integral;
    int32_t derivative;

    int32_t target;
    int32_t current;
    // 积分限幅
    int32_t max_integral;
    int32_t output;
    // 输出限幅
    int32_t max_output;
    int32_t Kp;
    int32_t Ki;
    int32_t Kd;
    
}PID;

/// @brief PID的通用控制函数
/// @param pid 传入的PID结构体
void __attribute__((weak)) PID_Update(PID *pid) 
{
    // 众所周知，PID的计算总是需要当前值的，如果用到这个函数，别忘了给他当前值哦！！
    pid->current = NULL;

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


void PID_TypeStructInit(PID *pid,int16_t kp,int16_t kd,int16_t ki,int16_t target);
void PID_forLine(PID *pid);
void PID_forTurn(PID *pid);
void speedControl(PID *pid);

#endif // __PID__