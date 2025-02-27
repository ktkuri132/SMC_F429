#include <BSP/usart/Serial.h>
#include <LIB/PID/pid.h>
#include <Project/LIB/Control/control.h>
#include <Project/Project.h> // include the project header file
#include <stdint.h>
#include <sys/types.h>

/*
 *  既然题目看错了，那我就不管了，直接开始堆史山吧
 *
 */


/*
 *  函数返回规则：1 成功，0 失败
 *
 *  启动流程：
 *        1. 检查数字是否成功存储
 *              1.1. 装载药瓶之前的读取的数字都是要存储的数字
 *              1.2.
 * 根据存储数字的个数决定药房的位置，1个数字近端病房，2个数字中间病房，3个数字远端病房
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

extern Stde_DataTypeDef USART2_DataBuff;
extern Stde_DataTypeDef USART3_DataBuff;
extern Stde_DataTypeDef UART5_DataBuff;
extern Stde_DataTypeDef UART4_DataBuff;
extern ctrl *Base;
extern nctrl *Near;

void __ControlTask(){
    Base->Data_Save_from_Camer();
    switch (Base->Key_Value) {
        case 1:
            Near->nearControl();
            break;

        default:
            break;
    }
    Project_LIB_ControlTask(Near->Base->RLControl);
}


#ifdef OLD_CODE
extern uint8_t CamerVerify[4];
extern uint8_t SaveDataLock;

uint8_t RLControl = 0;
uint8_t SiteLock  = 0;

/// @brief
/// 只要进入转向选择,运行一次后,无法继续运行,除非SiteLock被解锁,解锁的条件是OpenMV识别到十字路口,数据类型返回3
uint8_t __attribute__((__weak__)) Temp_Dire_select()
{
    uint8_t Temp_RLControl = 0;
    if (CamerVerify[0])
    {
        Temp_RLControl = CamerVerify[1];
    }
    if (CamerVerify[2])
    {
        Temp_RLControl = CamerVerify[3];
    }
    return Temp_RLControl;
}

uint8_t Temp_RLContrl = 0;
uint8_t Turn_const    = 0;
extern uint8_t VerifyDataLock;
uint8_t old_RLControl = 0;
void __attribute__((__weak__)) Dire_select(uint8_t Temp)
{
    old_RLControl = RLControl;
    if (SiteLock == 3)
    {
        if (Temp)
        {
            RLControl = Temp;
        }
    }
    else if (SiteLock == 1)
    {
        RLControl = 0;
    }
    else
    {
        RLControl = 0;
    }
    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (old_RLControl != RLControl)
    {
        Turn_const++;
        if (Turn_const == 2)
        { // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
            VerifyDataLock--;
            Turn_const    = 0;
            Temp_RLContrl = 0;
        }
    }
}

/*
    Data_Get_from_Camer识别到数字之后,这个坐标信息就不能再被改变
*/
/// @brief 控制状态
int8_t __attribute__((__weak__)) Project_LIB_ControlStrat()
{
    int8_t dsfc_return = Data_Save_from_Camer();
    if (dsfc_return > 0)
    {
        static uint8_t i = 0; // 此处强调验证数据锁的初始化只能进行一次
        if (!i)
        {
            VerifyDataLock = (SaveDataLock) ? ((SaveDataLock == 1) ? 1 : 2) : 0;
            i              = 1;
        }
    }
    // 验证数据锁还有次数就还能继续运行
    if (VerifyDataLock)
    {
        int8_t dgfc_return = Data_Get_from_Camer(); // 只要识别到数字,才能进入转向选择

        if (dgfc_return > 0)
        { // 大于0表示识别到数字
            Temp_RLContrl = Temp_Dire_select();
        }
        Dire_select(Temp_RLContrl);
    }
    Project_LIB_ControlTask();
}
#endif

/*
    关于转向的细节:
        1.k210识别到数字之后,这个坐标信息就不能再被改变,除非转向过程结束
        2.如何检测转向过程结束?
            2.1.用MPU6050,在接受到转向指令后,取当前的yaw值,然后在转向过程中,不断的取yaw值,当yaw值与初始值相差一定角度时,认为转向结束
            2.2.用Openmv,在转向期间,观察是否扫到分叉路的直线,扫到直线就开始走直线,并认为转向结束
        3.1.假如是有两个数字,检测到转向结束后,坐标信息就可以被改变了,开启识别下一个数字的流程
        3.2.假如是只有一个数字,转向结束进入返回模式,由新的系统控制
*/


extern int8_t Rvalue, Lvalue;

/// @brief 控制任务
void Project_LIB_ControlTask(uint8_t rlControl)
{
    static PID pidForLine; // 创建PID结构体
    static PID pidforspeed;
    static PID pidforturn;

    PID_TypeStructInit(&pidforspeed, 400, -10, 2, 16); // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 8, -8, 0, 180);    // 初始化寻线PID
    PID_TypeStructInit(&pidforturn, 500, -10, 0, 50);  // 为转向时不受电池电量影响

    pidForLine.PID_Update1  = PID_forLine;
    pidforspeed.PID_Update1 = speedControl;
    pidforturn.PID_Update1  = TurnControl;

    pidForLine.PID_Update1(&pidForLine);
    pidforspeed.PID_Update1(&pidforspeed);

    // 1000 6000
    if (rlControl == 2) // 左拐
    {
        pidforturn.PID_Update1(&pidforturn);
        Project_LIB_Motor_Load(pidforturn.output, 0);
    }
    else if (rlControl == 1) // 右拐
    {
        pidforturn.PID_Update1(&pidforturn);
        Project_LIB_Motor_Load(0, pidforturn.output);
    }
    else if (rlControl == 3) // 直线
    {
        Project_LIB_Motor_Load(pidforspeed.output, pidforspeed.output);
    }
    else
    {
        Project_LIB_Motor_Load(pidforspeed.output + pidForLine.output,
                               pidforspeed.output - pidForLine.output); // 装载到电机
    }
}

