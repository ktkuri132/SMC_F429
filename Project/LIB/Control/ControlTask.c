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
extern PET *Pet;

PID_arg PID_arg1 = {180, 20};

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
            if ((Base->j == 2) && (!Base->Back_sign)) {
                if (Base->CamerVerify[2] != 0) {
                    if (Base->Back_sign != 3) {
                        Base->Key_Value = 3;
                    }
                }
            }
        } break;
        case 3: {
            static uint8_t i = 0;
            if (!i) {
                Far = Far_Struct_Inti();  // 继承控制结构体
                Pet = Pet_Struct_Inti();
                i   = 1;
            }
            Far->farControl();
            /*模式重新选择*/
            if (Base->SiteLock == 3) {
                if ((!Base->CamerVerify[0]) && (!Pet->SDL)) {
                    Base->Key_Value = 4;
                }
            }
        } break;
        case 4: {
            PathExceptionHandler();
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

    PID_TypeStructInit(&pidforspeed, 400, 10, 0, PID_arg1.speed_target);  // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 8,10 , 0, PID_arg1.line_target);  // 初始化寻线PID,目标值：中线坐标

    pidForLine.PID_Update1  = PID_forLine;
    pidforspeed.PID_Update1 = speedControl;

    pidForLine.PID_Update1(&pidForLine);
    pidforspeed.PID_Update1(&pidforspeed);

    if (rlControl == 2)  // 左拐
    {
        Base->Motor_Load(4200, 500);
    } else if (rlControl == 1)  // 右拐
    {
        Base->Motor_Load(500, 4200);
    } else if (rlControl == 3)  // 调头
    {
        Base->Motor_Load(-1700, 1700);
    } else if (rlControl == 4) {  // 停车
        Base->Motor_Load(0, 0);
    } else if (rlControl == 6) {
        if(Base->Key_Value == 3){
            Base->Motor_Load(0, 4000);
        }
    } else if (rlControl == 5) {
        if(Base->Key_Value == 3){
            Base->Motor_Load(4000, 0);
        }
    } else {
        Base->Motor_Load(pidforspeed.output + pidForLine.output,
                         pidforspeed.output - pidForLine.output);  // 装载到电机
    }
}
