#include <Project.h>    // include the project header file
#include "control.h"

extern Stde_DataTypeDef USART3_DataBuff;

/// @brief 控制状态
void Project_LIB_ControlStrat()
{
    //* 检查当前药品装载情况
    if(Project_BSP_HW201_Get())
    {
        //* 药品装载完毕
        Project_LIB_ControlTask();
    }
    else 
    {
        //* 药品未装载
        Project_LIB_Motor_Load(0,0);
    }

}

/// @brief 控制任务
void Project_LIB_ControlTask()
{
    
    static PID pidForLine;                                              //* 创建PID结构体
    PID_TypeStructInit(&pidForLine,150,-10,0,120,PID_forLine,NULL);     //* 初始化

    pidForLine.PID_Update1(&pidForLine);                                //* 计算
    Project_LIB_Motor_Load(pidForLine.output,pidForLine.output);        //* 装载到电机

}


// @brief deal the data from the USART and these data will be from OpenMV or V831
void Data_Deal()
{

}