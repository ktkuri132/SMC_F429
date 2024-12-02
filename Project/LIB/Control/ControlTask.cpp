#include <Project.h>    // include the project header file
#include "control.h"

extern Stde_DataTypeDef USART3_DataBuff;

void Project_LIB_ControlTask()
{
    PID *pidForLine;
    pidForLine->target = 120;
    pidForLine->current = USART_Deal(&USART3_DataBuff,1);
    pidForLine->Kp = 20;
    pidForLine->Kd = -50;
    pidForLine->PID_forLine();
    Project_LIB_Motor_Load(pidForLine->output,pidForLine->output);

}


// @brief deal the data from the USART and these data will be from OpenMV or V831
void Data_Deal()
{

}