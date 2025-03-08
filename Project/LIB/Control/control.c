#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>

#include "Project.h"

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
extern fctrl *Far;
CrossManage Cross[5];  // 十字路口管理,主要用于返回

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
        .nearControl      = __nearControl,
        .Dire_Load_ENABLE = 1,
    };

    // 避免静态变量的初始化不是常量
    near.Base                 = Base;
    near.Base->VerifyDataLock = 1;
    near.Base->Turn_const     = 1;
    return &near;
}

mctrl *Min_Struct_Inti() {
    static mctrl min = {
        .minControl       = __minControl,
        .Temp_Dire_select = __Temp_Dire_select,
        .Dire_Load_ENABLE = 1,
    };

    // 避免静态变量的初始化不是常量
    min.Base                      = Base;
    min.Base->VerifyDataLock      = 1;
    min.Base->Turn_const          = 3;
    min.Base->Data_Get_from_Camer = __Data_Get_from_Camer;
    return &min;
}

fctrl *Far_Struct_Inti() {
    static fctrl far = {
        .farControl   = __farControl,
        .SaveDataLock = 0,
    };

    // 避免静态变量的初始化不是常量
    far.Base = Base;
    return &far;
}

/// @brief 从摄像头获取数字
int8_t __Data_Save_from_Camer() {
    static TaskHandle_t *Task4_LEDPlayR_Handle;
    static TaskHandle_t *Task4_LEDPlayY_Handle;
    static uint8_t i = 0;
    if (!Base->SaveDataLock) {
        static uint8_t TempNum, TempSite;
        if (!K210Data) {
            if (Base->MotorStrat_2) {  // 此时若电机被启动，直接强行锁定
                Base->SaveDataLock = 1;
                return -1;
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
                    return 2;
                }
                return 1;
            }
        } else {        // 程序运行到这里，说明出现了扫描空挡
            return -1;  // 返回3只会出现在，第一次出现扫描空挡的瞬间
        }
    }
    return 1;
}

/// @brief 从摄像头验证数字
int8_t __Data_Get_from_Camer() {
    if (Base->SaveDataLock) {
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
        }
        return -1;
    }
}

/// @brief 近端病房模式
void __nearControl() {
    if (Base->SiteLock == 1) {
        if (Near->Turn_start) {
            Base->Temp_RLContrl    = 0;
            Near->Turn_start       = 0;
            Near->Dire_Load_ENABLE = 0;
        }
    }
    // 返回状态不允许操作Temp_RLContrl，阉割版临时转向控制
    if (Near->Dire_Load_ENABLE) {
        if (!Base->Back_sign) {
            if (Base->CamerData[0] == 1) {
                Base->Temp_RLContrl = 2;
            } else if (Base->CamerData[0] == 2) {
                Base->Temp_RLContrl = 1;
            }
        }
    }
    // 返回执行函数
    Base->Back();
    // 正式转向控制
    __Dire_select(Base->Temp_RLContrl);

    // 已返回状态下，触发遇黑色和白色暂停
    if (Base->Back_sign == 3) {
        if (Base->SiteLock == 2 || Base->SiteLock == 4) {
            Base->RLControl = 4;
        }
    }
}

/// @brief 中端病房模式（兼容远端病房）
void __minControl() {
    // 从摄像头验证数字
    Base->Data_Get_from_Camer();
    // 十字路口记录
    __CrossManage();
    // 临时转向控制
    Min->Temp_Dire_select();

    // 返回执行函数
    Base->Back();

    // 已返回状态下，触发遇黑色和白色暂停
    if (Base->Back_sign == 3) {
        if (Base->SiteLock == 2 || Base->SiteLock == 4) {
            Base->RLControl = 4;
        }
    }
    // 正式转向控制
    __Dire_select(Base->Temp_RLContrl);

    if (Base->Key_Value == 3) {
        MTurn_Strat();
    }
}

/// @brief 远端病房模式（以弃用）
void __farControl() {  // 确认是不是真的没有正确的数字
}

/// @brief 转向状态记录函数
void MTurn_Strat() {
    static uint8_t i             = 0;
    static uint8_t Temp_SiteLock = 1;
    if (Base->Back_sign == 3) {
        if (i < 5) {
            if (Base->SiteLock == 5 || Base->SiteLock == 6) {
                if (Base->SiteLock != Temp_SiteLock) {
                    i++;
                    Temp_SiteLock = Base->SiteLock;
                    if (Base->SiteLock == 5) {
                        Base->RLControl = 2;
                    } else if (Base->SiteLock == 6) {
                        Base->RLControl = 1;
                    }
                }
            } else if (Base->SiteLock == 1) {
                if (i == 1) {
                    Temp_SiteLock = 1;
                }
            }
        }
    }
}

uint8_t __CrossManageNum() {
    static uint8_t i = 0;
    if (!i) {
        if (Base->SiteLock == 1) {
            i = 1;
            return Base->j;
        }
    }
    if (Base->SiteLock == 3) {
        if (i) {
            i = 2;
            return Base->j;
        }
    }
    if (Base->SiteLock == 1) {
        if (i == 2) {
            Base->j++;
            i = 0;
            return Base->j;
        }
    }
}

uint8_t __Cross

/// @brief 临时转向控制函数
uint8_t __Temp_Dire_select() {
    static uint8_t i = 0;
    if (Base->Back_sign != 3) {  // 小车宣布返回前，直接根据摄像头数据进行转向
        if (Base->CamerVerify[0]) {
            Base->Temp_RLContrl = Base->CamerVerify[1];
        }
    } else {                        // 宣布返回后，根据之前的转向记录处理转向
        if (Base->Temp_RLContrl) {  // 假如小车之前有转向记录
            if (Base->CamerVerify[1] == 1) {
                Base->Temp_RLContrl = 2;
            } else if (Base->CamerVerify[1] == 2) {
                Base->Temp_RLContrl = 1;
            }
        }
    }
    if (Base->Back_sign == 3) {  // 返回状态下
        // 扫到十字路口，使能Turn_const
        if (Base->SiteLock == 3) {
            Min->Turn_const = 1;
        } else if (Base->SiteLock == 1) {
            if (Min->Turn_const) {
                Base->Temp_RLContrl =
                    0;  // 在在经过十字路口后，再次扫到直线，说明转向结束，此时应该不转向
            }
        }
    }
    return 0;
}

void __Dire_select(uint8_t Temp) {
    static uint8_t Turn_const = 0;
    Base->old_RLControl       = Base->RLControl;
    if (Base->SiteLock == 3) {
        if (Temp) {
            Base->RLControl = Temp;
        }
        static uint8_t i = 0;
        if (!i) {
            Base->VerifyDataLock = 0;
            i                    = 1;
        }
    } else if (Base->SiteLock == 1) {
        Base->RLControl = 0;
    }
    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (Base->old_RLControl != Base->RLControl) {
        Turn_const++;

        if (Turn_const ==
            Base->Turn_const) {  // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
            Near->Turn_start = 1;
            Base->VerifyDataLock ? Base->VerifyDataLock-- : 0;
        }
    }
}

/// @brief 返回处理函数
void __Back() {
    if (!Base->VerifyDataLock) {      // 当验证数据锁为0时，说明已经经过路口，此时激活返回控制函数
        if (Base->SiteLock == 4) {    // 假如扫到黑线
            if (!Base->Back_sign) {   // 且保证为第一次激活返回控制函数
                Base->Back_sign = 1;  // 返回控制进入第一阶段：暂停取药
                if (Base->Key_Value == 1) {  // 记录之前的转向状态，专为近端病房服务
                    Base->Temp_RLContrl = Base->CamerData[0];
                }
            }
        }
        if (Base->Back_sign == 1) {  // 在第一阶段暂停取药时，保证小车停止
            Base->RLControl = 4;
        }
        if (!Base->MotorStrat_2) {                           // 假如处于第一阶段，检测到药品被拿走
            Base->Back_sign = 2;                             // 就进入第二阶段：调头
            if (Base->SiteLock != 1) {                       // 调头的时候，保证小车具有一定的特权
                Base->RLControl          = 3;                // 发出调头指令
                Base->MotorStrat_3_POINT = 1;                // 不受白色背景停止的影响
                Base->Motor_Load         = back_Motor_Load;  // 小车启动条件改为：无药品启动
                Base->VerifyDataLock     = 1;                // 第二阶段初始化完毕，验证数据锁为1
            }
        }
        return;  // 小车在遇到红线之前，一直都是第二阶段
    }
    if (Base->SiteLock == 1) {             // 遇到红线，进入第三阶段
        if (Base->Back_sign == 2) {        // 保证是第二阶段结束
            Base->MotorStrat_3_POINT = 0;  // 使得小车受白色背景停止的影响
            Base->Back_sign = 3;  // 进入第三阶段：返回，此刻返回处理函数使命结束，宣布开始返回
        }
    }
}
