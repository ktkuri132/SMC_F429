#include <Project.h>
#include <control.h>


/*
 * 这里为什么要提供一个通用，还建议开发者提供专用的PID算法，通用的只有单极PID
 * 但是专用算法的可以随意发挥，并机，串级，这里提供了两个PID算法的接口，可供开发者提供最多两级的算法支持
 * 如有更多需求，可在结构体内添加，初始化函数中加入接口
 */

void PID_TypeStructInit(PID *pid,int16_t kp,int16_t kd,int16_t ki,int16_t target,void *lineControl,void *turnControl)
{
    pid->max_output = MAX_PWM;
    pid->max_integral = 7200;
    pid->integral = 120;
    pid->target = target;
    pid->Kp = kp;
    pid->Kd = kd;
    pid->Ki = ki;
    if(lineControl == NULL)
    {
        pid->PID_Update1 = PID_Update;
    }
    else
    {
        pid->PID_Update1 = lineControl;
    }
    if(turnControl == NULL)
    {
        pid->PID_Update2 = PID_Update;
    }
    else 
    {
        pid->PID_Update2 = turnControl;
    }
}
