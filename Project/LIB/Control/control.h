#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>


#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

class PID
{
private:
    /* data */
    int32_t error;
    int32_t last_error;
    int32_t integral;
    int32_t derivative;
public:
    int32_t target;
    int32_t current;
    // 积分限幅
    int32_t max_integral = MAX_PWM;
    int32_t output;
    // 输出限幅
    int32_t max_output = MAX_PWM;
    int32_t Kp;
    int32_t Ki;
    int32_t Kd;
    void PID_forLine();
};

#ifdef _cplusplus
}
#endif 




#endif // !_CONTROL_