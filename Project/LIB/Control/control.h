#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>


typedef struct PID_TypeDef
{
    /* function  */
    void (*PID_forLine)(PID *pid);
    void (*forlineparam);
    void (*PID_forTurn)(PID *pid);
    void (*forturnparam);
    uint8_t SchedulerSign;
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




#endif // !_CONTROL_