#include <Project.h>    // include the project header file

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;    // declare the data buffer

/*
 *  函数返回规则：1 成功，0 失败
 *
 *  启动流程：
 *        1. 检查数字是否成功存储
 *              1.1. 装载药瓶之前的读取的数字都是要存储的数字
 *              1.2. 根据存储数字的个数决定药房的位置，1个数字近端病房，2个数字中间病房，3个数字远端病房    
 *        2. 检查药品是否装载完毕（装载药瓶之后的读取的数字都是要比较的数字）
 *  正常运行流程：
 *        1. OpenMV扫描
 *        2. 检查是否扫到十字路口
 *              2.1. 扫到十字路口，停车
 *              2.2. 打开串口通信，等待接收数字识别数据
 *              2.3. 接收到数据，与存储的数据进行比对，决定转向
 *              2.4. 读取MPU6050，开始转向
 *        3. 检查是否扫到十字路口
 *
 */


/// @brief 控制状态
uint8_t Project_LIB_ControlStrat()
{

    static uint8_t CamerData;
CheckNumber:
    //* 检查数据是否到达
    if(Data_Save_from_Camer(&CamerData))
    {
        goto CheckMedical;
    }
    else
    {
        goto ControlRun;
    } 

CheckMedical:
    //* 检查当前药品装载情况
    while(Project_BSP_HW201_Get)
    {
        Project_LIB_Motor_Load(0,0);   
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
    PID_TypeStructInit(&pidForLine,10,-10,0,120,PID_forLine,NULL);     //* 初始化

    pidForLine.PID_Update1(&pidForLine);    
    // printf("%d\n",pidForLine.output);                            //* 计算                             //* 发送数据
    Project_LIB_Motor_Load(3000-pidForLine.output,3000+pidForLine.output);        //* 装载到电机

}


