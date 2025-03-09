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
            /*模式重新选择*/
            if(Base->j >= 3){
                if(Base->Back_sign != 3){
                    Base->Key_Value = 3;
                }
            }
        } break;
        case 3: {
            static uint8_t i = 0;
            if (!i) {
                Far = Far_Struct_Inti();  // 继承控制结构体
                i   = 1;
            }
            Far->farControl();
        } break;
        default:
            break;
    }
    Project_LIB_ControlTask(Base->RLControl);
}


/// @brief 控制任务
void Project_LIB_ControlTask(uint8_t rlControl) {
    static PID pidForLine;  // 创建PID结构体
    static PID pidforspeed;
    static PID pidforturn;
    static PID pidForback;

    PID_TypeStructInit(&pidforspeed, 400, -10, 2, 21);  // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 10, -7, 0, 160);     // 初始化寻线PID,目标值：中线坐标
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
