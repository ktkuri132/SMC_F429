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
    int16_t error;
    int16_t last_error;
    int16_t integral;
    int16_t derivative;
public:
    int16_t target;
    int16_t current;
    // 积分限幅
    int16_t max_integral = MAX_PWM;
    int16_t output;
    // 输出限幅
    int16_t max_output = MAX_PWM;
    int16_t Kp;
    int16_t Ki;
    int16_t Kd;
    void PID_forLine();
};




#ifdef _cplusplus
}
#endif 

#endif // !_CONTROL_