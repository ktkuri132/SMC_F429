#include <Project.h>    // include the project header file

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;    // declare the data buffer

/// @brief 控制状态
uint8_t Project_LIB_ControlStrat()
{

CheckNumber:
    //* 检查数据是否到达
    if(V831Data)
    {
        //* 数据到达,存储数据
        Data_Save_from_Camer();
        goto CheckMedical;
    }
    else 
    {   
        //* 数据未到达
        goto CheckNumber;
    }


CheckMedical:
    //* 检查当前药品装载情况
    if(Project_BSP_HW201_Get())
    {
        //* 药品装载完毕
        goto ControlRun;
    }
    else 
    {
        //* 药品未装载
        Project_LIB_Motor_Load(0,0);
        goto CheckMedical;
    }


ControlRun:
    //* 控制任务
    Project_LIB_ControlTask();

    goto CheckNumber;

    return 0;


}

/// @brief 控制任务
void Project_LIB_ControlTask()
{
    
    static PID pidForLine;                                              //* 创建PID结构体
    PID_TypeStructInit(&pidForLine,150,-10,0,120,PID_forLine,NULL);     //* 初始化

    pidForLine.PID_Update1(&pidForLine);                                //* 计算
    Project_LIB_Motor_Load(pidForLine.output,pidForLine.output);        //* 装载到电机

}


