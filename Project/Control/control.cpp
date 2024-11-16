#include <Project.h>
#include <control.h>

// 基础PID算法
void PID::PID_forLine()
{
    this->error = this->target - this->current;
    this->integral += this->error;
    this->integral *= this->Ki;
    // 积分限幅
    if(this->integral > this->max_integral)
    {
        this->integral = this->max_integral;
    }
    else if(this->integral < -this->max_integral)
    {
        this->integral = -this->max_integral;
    }
    this->derivative = this->error - this->last_error;
    this->output = this->Kp * this->error + this->Ki * this->integral + this->Kd * this->derivative;
    // 输出限幅
    if(this->output > this->max_output)
    {
        this->output = this->max_output;
    }
    else if(this->output < -this->max_output)
    {
        this->output = -this->max_output;
    }
    this->last_error = this->error;
}
