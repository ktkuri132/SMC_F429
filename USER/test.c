#include "Dev/TB6612/tb6612_port.h"
#include <BSP/usart/Serial.h>
#include <HARDWARE/MPU6050/inv_mpu.h>
#include <OLED/OLED.h>
#include <Project/LIB/PID/pid.h>
#include <stdio.h>

extern float pitch, roll, yaw;
static uint8_t Temp_RLContrl = 0;

void Test_Turn()
{
}

extern int8_t Rvalue, Lvalue;

void Test()
{
    Motor->Left  = 2000;
    Motor->Right = 2000;
    AllForward
    while (1)
    {
        OLED_Printf(0, 24, OLED_6X8, "MPU6050:%f", yaw);
        OLED_Update();
        printf("Rvalue:%d,Lvalue:%d\n", Rvalue, Lvalue);
    }
}




void Control_Speed()
{
    static PID Line;  // 创建PID结构体
    static PID speed;

    
    PID_TypeStructInit(&Line, 10, -10, 0, 150);  // 初始化
    PID_TypeStructInit(&speed, 0.5, 0, 0, 16);  // 初始化
    Line.PID_Update1 = PID_forLine;
    speed.PID_Update1 = speedControl;

    
    Line.PID_Update1(&Line);
    speed.PID_Update1(&speed);

    Project_LIB_Motor_Load(speed.output - Line.output,
                            speed.output + Line.output);  // 装载到电机
    
}