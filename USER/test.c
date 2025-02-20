#include <BSP/usart/Serial.h>
#include <HARDWARE/MPU6050/inv_mpu.h>
#include <OLED/OLED.h>
#include <Project/LIB/PID/pid.h>


extern float pitch, roll, yaw;
static uint8_t RLContrl = 0;

void Turn_Control()
{
    static PID pidForLine;                                               // 创建PID结构体
    PID_TypeStructInit(&pidForLine, 10, -10, 0, 120, PID_forLine, NULL); // 初始化

    pidForLine.PID_Update1(&pidForLine);
    // printf("%d\n",pidForLine.output);
    if (RLContrl == 2)
    {
        Project_LIB_Motor_Load(-2000, 2000);
    }
    else if (RLContrl == 1)
    {
        Project_LIB_Motor_Load(2000+800, -2000+800);
    }
    else
    {
        Project_LIB_Motor_Load(2000 - pidForLine.output, 2000 + pidForLine.output); // 装载到电机
    }
}

void Test_Turn()
{

}


void Test()
{

    OLED_Printf(0, 24, OLED_6X8, "MPU6050:%f", yaw);
    OLED_Update();
}