#include <BSP/usart/Serial.h>
#include <HARDWARE/MPU6050/inv_mpu.h>
#include <OLED/OLED.h>
#include <Project/LIB/PID/pid.h>


extern float pitch, roll, yaw;
static uint8_t Temp_RLContrl = 0;



void Test_Turn()
{

}


void Test()
{

    OLED_Printf(0, 24, OLED_6X8, "MPU6050:%f", yaw);
    OLED_Update();
}