#include <Project/Project.h> // include the project header file
#include <BSP/usart/Serial.h>
#include <LIB/PID/pid.h>
#include <stdint.h>
extern Stde_DataTypeDef USART3_DataBuff, UART5_DataBuff, UART4_DataBuff; // declare the data buffer

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


extern uint8_t CamerVerify[4]; 
extern uint8_t DataLock;

static uint8_t RLContrl;

uint8_t SiteLock = 0;

/// @brief 只要进入转向选择,运行一次后,无法继续运行,除非SiteLock被解锁,解锁的条件是OpenMV识别到十字路口,数据类型返回3
void Dire_select(){
    do{
        if(CamerVerify[0])
        {
            RLContrl = CamerVerify[1];
        }
        if(CamerVerify[2])
        {
            RLContrl = CamerVerify[3];
        }
    }
    while (SiteLock);
}
/*
    Data_Get_from_Camer识别到数字之后,这个坐标信息就不能再被改变
*/
/// @brief 控制状态
int8_t Project_LIB_ControlStrat()
{
    Data_Save_from_Camer();
    int8_t dgfc_return =  Data_Get_from_Camer();    // 只要识别到数字,才能进入转向选择
    if (dgfc_return > 0) {
        Dire_select();
    }
    Project_LIB_ControlTask();
}


/*
    关于转向的细节:
        1.k210识别到数字之后,这个坐标信息就不能再被改变,除非转向过程结束
        2.如何检测转向过程结束?
            2.1.用MPU6050,在接受到转向指令后,取当前的yaw值,然后在转向过程中,不断的取yaw值,当yaw值与初始值相差一定角度时,认为转向结束
            2.2.用Openmv,在转向期间,观察是否扫到分叉路的直线,扫到直线就开始走直线,并认为转向结束
        3.1.假如是有两个数字,检测到转向结束后,坐标信息就可以被改变了,开启识别下一个数字的流程
        3.2.假如是只有一个数字,转向结束进入返回模式,由新的系统控制
*/

void Turn()
{
    
}

/// @brief 控制任务
void Project_LIB_ControlTask()
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
