#include <BSP/usart/Serial.h>
#include <LIB/PID/pid.h>
#include <Project/LIB/Control/control.h>
#include <Project/Project.h>  // include the project header file
#include <stdint.h>
#include <sys/types.h>

extern Stde_DataTypeDef USART2_DataBuff;
extern Stde_DataTypeDef USART3_DataBuff;
extern Stde_DataTypeDef UART5_DataBuff;
extern Stde_DataTypeDef UART4_DataBuff;
extern ctrl *Base;
extern nctrl *Near;
extern mctrl *Min;
extern fctrl *Far;

void Mode_chose() {
    if (Base->CamerData[0]) {
        if (Base->CamerData[0] == 1 || Base->CamerData[0] == 2) {
            Base->Key_Value = 1;
        } else {
            Base->Key_Value = 2;
        }
    }
}

void __ControlTask() {
    int8_t dsfc_return = Base->Data_Save_from_Camer();
    if (dsfc_return < 0) {
        return;
    }
    static uint8_t i = 0;
    if (!i) {
        Mode_chose();
        i++;
    }
    switch (Base->Key_Value) {
        case 1: {
            static uint8_t i = 0;
            if (!i) {
                Near = Near_Struct_Inti();  // 继承控制结构体
                i    = 1;
            }
            Near->nearControl();
        } break;
        case 2: {
            static uint8_t i = 0;
            if (!i) {
                Min = Min_Struct_Inti();  // 继承控制结构体
                i   = 1;
            }
            Min->minControl();
        } break;
        case 3: {
            // static uint8_t i = 0;
            // if (!i) {
            //     Far = Far_Struct_Inti();  // 继承控制结构体
            //     i   = 1;
            // }
            // Far->farControl();
            Min->minControl();
        } break;
        default:
            break;
    }
    Project_LIB_ControlTask(Base->RLControl);
}

#ifdef OLD_CODE
extern uint8_t CamerVerify[4];
extern uint8_t SaveDataLock;

uint8_t RLControl = 0;
uint8_t SiteLock  = 0;

/// @brief
/// 只要进入转向选择,运行一次后,无法继续运行,除非SiteLock被解锁,解锁的条件是OpenMV识别到十字路口,数据类型返回3
uint8_t __attribute__((__weak__)) Temp_Dire_select() {
    uint8_t Temp_RLControl = 0;
    if (CamerVerify[0]) {
        Temp_RLControl = CamerVerify[1];
    }
    if (CamerVerify[2]) {
        Temp_RLControl = CamerVerify[3];
    }
    return Temp_RLControl;
}

uint8_t Temp_RLContrl = 0;
uint8_t Turn_const    = 0;
extern uint8_t VerifyDataLock;
uint8_t old_RLControl = 0;
void __attribute__((__weak__)) Dire_select(uint8_t Temp) {
    old_RLControl = RLControl;
    if (SiteLock == 3) {
        if (Temp) {
            RLControl = Temp;
        }
    } else if (SiteLock == 1) {
        RLControl = 0;
    } else {
        RLControl = 0;
    }
    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (old_RLControl != RLControl) {
        Turn_const++;
        if (Turn_const == 2) {  // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
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
int8_t __attribute__((__weak__)) Project_LIB_ControlStrat() {
    int8_t dsfc_return = Data_Save_from_Camer();
    if (dsfc_return > 0) {
        static uint8_t i = 0;  // 此处强调验证数据锁的初始化只能进行一次
        if (!i) {
            VerifyDataLock = (SaveDataLock) ? ((SaveDataLock == 1) ? 1 : 2) : 0;
            i              = 1;
        }
    }
    // 验证数据锁还有次数就还能继续运行
    if (VerifyDataLock) {
        int8_t dgfc_return = Data_Get_from_Camer();  // 只要识别到数字,才能进入转向选择

        if (dgfc_return > 0) {  // 大于0表示识别到数字
            Temp_RLContrl = Temp_Dire_select();
        }
        Dire_select(Temp_RLContrl);
    }
    Project_LIB_ControlTask();
}
#endif

void scan(){

}

/// @brief 控制任务
void Project_LIB_ControlTask(uint8_t rlControl) {
    static PID pidForLine;  // 创建PID结构体
    static PID pidforspeed;
    static PID pidforturn;
    static PID pidForback;

    PID_TypeStructInit(&pidforspeed, 400, -10, 2, 20);  // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 8, -8, 0, 180);     // 初始化寻线PID,目标值：中线坐标
    PID_TypeStructInit(&pidforturn, 500, -10, 0, 40);   // 为转向时不受电池电量影响
    PID_TypeStructInit(&pidForback, 10, -10, 0, 1500);  // 为调头时不受电池电量影响

    pidForLine.PID_Update1  = PID_forLine;
    pidforspeed.PID_Update1 = speedControl;
    pidforturn.PID_Update1  = TurnControl;
    pidForback.PID_Update1  = BackControl;

    pidForLine.PID_Update1(&pidForLine);
    pidforspeed.PID_Update1(&pidforspeed);

    // 1000 6000
    if (rlControl == 2)  // 左拐
    {
        // pidforturn.PID_Update1(&pidforturn);
        // Base->Motor_Load(pidforturn.output, 0);
        Base->Motor_Load(2000, 0);
    } else if (rlControl == 1)  // 右拐
    {
        // pidforturn.PID_Update1(&pidforturn);
        // Base->Motor_Load(0, pidforturn.output);
        Base->Motor_Load(0, 2000);
    } else if (rlControl == 3)  // 调头
    {
        Base->Motor_Load(-1700, 1700);
    } else if (rlControl == 4) {  // 停车
        Base->Motor_Load(0, 0);
    } 
    else if(rlControl == 5){
        Base->Motor_Load(0, 2000);
    }
    else if(rlControl == 6){
        Base->Motor_Load(2000, 0);
    }
    else {
        Base->Motor_Load(pidforspeed.output + pidForLine.output,
                         pidforspeed.output - pidForLine.output);  // 装载到电机
    }
}
