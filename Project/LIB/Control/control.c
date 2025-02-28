#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>
#include <stdio.h>

#include "LIB/PID/pid.h"
#include "Project.h"
#include "stm32f4xx.h"

/*
 * 这里为什么要提供一个通用，还建议开发者提供专用的PID算法，通用的只有单极PID
 * 但是专用算法的可以随意发挥，并机，串级，这里提供了两个PID算法的接口，可供开发者提供最多两级的算法支持
 * 如有更多需求，可在结构体内添加，初始化函数中加入接口
 */

extern Stde_DataTypeDef USART2_DataBuff;
extern Stde_DataTypeDef USART3_DataBuff;
extern Stde_DataTypeDef UART5_DataBuff;
extern Stde_DataTypeDef UART4_DataBuff;
extern ctrl *Base;
extern nctrl *Near;
extern mctrl *Min;

ctrl *Control_Struct_Inti() {
    static ctrl base = {
        .MotorStrat_1         = 1,
        .MotorStrat_2         = 0,
        .MotorStrat_3_POINT   = 1,
        .MotorStrat_3         = 0,
        .Motor_Load           = forward_Motor_Load,
        .ControlTask          = __ControlTask,
        .Data_Save_from_Camer = __Data_Save_from_Camer,
        .Dire_select          = __Dire_select,
        .Back                 = __Back,
    };
    return &base;
}

nctrl *Near_Struct_Inti() {
    static nctrl near = {
        .nearControl = __nearControl,
    };

    // 避免静态变量的初始化不是常量
    near.Base                 = Base;
    near.Base->VerifyDataLock = 1;
    near.Base->Turn_const     = 2;
    return &near;
}

mctrl *Min_Struct_Inti() {
    static mctrl min = {
        .minControl       = __minControl,
        .Temp_Dire_select = __Temp_Dire_select,
    };

    // 避免静态变量的初始化不是常量
    min.Base                      = Base;
    min.Base->VerifyDataLock      = 1;
    min.Base->Turn_const          = 4;
    min.Base->Data_Get_from_Camer = __Data_Get_from_Camer;
    return &min;
}

int8_t __Data_Save_from_Camer() {
    static TaskHandle_t *Task4_LEDPlayR_Handle;
    static TaskHandle_t *Task4_LEDPlayY_Handle;
    static uint8_t i = 0;
    if (!Base->SaveDataLock) {
        static uint8_t TempNum, TempSite;
        if (!K210Data) {
            if (Base->MotorStrat_2 || (Base->Key_Value == 1)) {  // 此时若电机被启动，直接强行锁定
                Base->SaveDataLock = 1;
                return i;
            }
            return -1;
        }
        if (TempNum == K210Data) {
            if (i == 2) {  // 存入一个数字，选择性锁定存储
                if (Base->MotorStrat_2 ||
                    (Base->Key_Value == 1)) {  // 此时若电机被启动，直接强行锁定
                    Base->SaveDataLock = 1;
                }
            }
            return -2;  //  上次和这次数据一样
        }
        TempNum  = K210Data;  // 刷新临时数据
        TempSite = K210Site;
        if (TempNum) {                              // 如果识别到了数字
            if (!Base->CamerData[i]) {              // 如果缓冲数组目前为止为空
                Base->CamerData[i]     = TempNum;   // 填入数字
                Base->CamerData[i + 1] = TempSite;  // 填入坐标
                if (!i) {                           // 返回0闪红灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Red_LED", 1024, 1, 10,
                                Task4_LEDPlayR_Handle);
                } else {  // 返回1闪黄灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Yellow_LED", 1024, 2, 10,
                                Task4_LEDPlayY_Handle);
                }
                i += 2;
                if (i == 4) {  // 存入两个数字，直接锁定数据存入
                    Base->SaveDataLock = 2;
                    return i;
                }
                return -1;
            }
        } else {        // 程序运行到这里，说明出现了扫描空挡
            return -1;  // 返回3只会出现在，第一次出现扫描空挡的瞬间
        }
    }
    return i;
}

int8_t __Data_Get_from_Camer() {
    if (Base->SaveDataLock) {
        static uint8_t m = 0;
        if (K210Data && !m) {
            return -1;
        }
        m = 1;
        if (!K210Data) {
            return -1;
        }
        if (Base->CamerData[0] == K210Data) {
            Base->CamerVerify[0] = K210Data;
            if (K210Site > 60) {
                Base->CamerVerify[1] = 1;
                return 1;  // 右边
            } else {
                Base->CamerVerify[1] = 2;
                return 2;  // 左边
            }
        } else {
            Base->Key_Value = 3;
        }

        return -1;  // 删除任务未完成
    }
}

void __nearControl() {
    if (!Base->Back_sign) {
        if (Base->CamerData[0] == 1) {
            Base->Temp_RLContrl = 2;
        } else if (Base->CamerData[0] == 2) {
            Base->Temp_RLContrl = 1;
        }
    }
    Base->Back();

    __Dire_select(Base->Temp_RLContrl);
}

void __minControl() {
    Base->Data_Get_from_Camer();

    Min->Temp_Dire_select();

    __Dire_select(Base->Temp_RLContrl);
}

uint8_t __Temp_Dire_select() {
    if (Base->CamerVerify[0]) {
        Base->Temp_RLContrl = Base->CamerVerify[1];
    }
    if (Base->CamerVerify[2]) {
        Base->Temp_RLContrl = Base->CamerVerify[3];
    }
    if (Base->Temp_RLContrl) {
        if (Base->Temp_RLContrl == 1) {
            Base->CamerData[0] = 1;
        } else if (Base->Temp_RLContrl == 2) {
            Base->CamerData[0] = 2;
        }
    }
    return 0;
}

void __Dire_select(uint8_t Temp) {
    static uint8_t Turn_const = 0;
    static uint8_t Turn_start = 0;
    static uint8_t Turn_end   = 0;
    Base->old_RLControl       = Base->RLControl;
    if (Base->SiteLock == 3) {
        if (Temp) {
            Base->RLControl = Temp;
        }
    } else if (Base->SiteLock == 1) {
        Base->RLControl = 0;
    }

    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (Base->old_RLControl != Base->RLControl) {
        Turn_const++;

        if (Turn_const ==
            Base->Turn_const) {  // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
            Base->VerifyDataLock ? Base->VerifyDataLock-- : 0;
            Turn_const          = 0;
            Base->Temp_RLContrl = 0;
            Base->CamerData[1]  = Base->CamerData[0];  // 备份转向数据
            Base->CamerData[0]  = 0;
        }
    }
}

void __Back() {
    if (!Base->VerifyDataLock) {
        if (Base->SiteLock == 4) {
            Base->Back_sign = 1;
            if (Base->CamerData[1] == 1) {
                Base->CamerData[0] = 2;
            } else if (Base->CamerData[1] == 2) {
                Base->CamerData[0] = 1;
            }
            Base->VerifyDataLock = 1;
            if (Base->SiteLock != 1) {
                Base->RLControl = 3;
            }
        }
    }
    if (Base->Back_sign) {
        if (Base->SiteLock == 1) {
            Base->Back_sign          = 0;
            Base->MotorStrat_3_POINT = 0;
            Base->Motor_Load         = back_Motor_Load;
        }
    }
}
