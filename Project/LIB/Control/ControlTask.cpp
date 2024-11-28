#include <Project.h>    // include the project header file
#include "control.h"

void ControlTask(void *pvParameters)
{
    PID *pidForLine = new PID();
    pidForLine->Kp = 200;
    pidForLine->Kd = -50;
    pidForLine->PID_forLine();
    Project_LIB_Motor_Load(pidForLine->output,pidForLine->output);

}


// @brief deal the data from the USART and these data will be from OpenMV or V831
void Data_Deal()
{

}