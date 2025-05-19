#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>
#include <string.h>

#include "Project.h"
#include "usart/Serial.h"

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
extern PET Pet;

extern PID_arg PID_arg1;

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
        .nearControl = __nearControl,
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
    };

    // 避免静态变量的初始化不是常量
    min.Base                      = Base;
    min.Base->VerifyDataLock      = 1;
    min.Base->Turn_const          = 3;
    min.Base->Data_Get_from_Camer = Data_Get_mid;
    return &min;
}

fctrl *Far_Struct_Inti() {
    static fctrl far = {
        .farControl       = __farControl,
        .Temp_Dire_select = __Temp_Dire_select,
    };
    // 避免静态变量的初始化不是常量
    far.Base                      = Base;
    far.Base->VerifyDataLock      = 1;
    far.Base->Data_Get_from_Camer = Data_Get_far;
    return &far;
}

PET *Pet_Struct_Inti() {
    static PET pet = {
        .lock       = 0,
        .temp       = 0,
        .PathNum    = 0,
        .SDL        = 0,
        .Error      = 0,
        .Runstate   = 0,
        .Runstate_2 = 0,
    };
    return &pet;
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
                i += 2;
                if (i == 4) {  // 存入两个数字，直接锁定数据存入
                    Base->SaveDataLock = 1;
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

int8_t Data_Get_mid() {
    if (Base->SaveDataLock) {
        if (!K210Data) {
            return -1;
        }
        if (!Base->CamerVerify[0]) {
            if (Base->CamerData[0] == K210Data) {
                Base->CamerVerify[0] = K210Data;
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 1;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                    return 1;  // 右边
                } else {
                    Base->CamerVerify[1] = 2;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                    return 2;  // 左边
                }
            }
        }
        return -1;
    }
}

/// @brief 从摄像头验证数字
int8_t Data_Get_far() {
    static uint8_t temp = 0;
    static uint8_t Num[4];
    static uint8_t i = 0;
    if (Base->SaveDataLock) {
        if (!K210Data) {
            return -1;
        }
        static uint8_t i = 0;
        if (!Base->CamerVerify[0]) {
            if (Base->CamerData[0] == K210Data) {
                Base->CamerVerify[0] = K210Data;
                Base->CamerVerify[2] = 0;
                Base->SaveDataLock   = 2;
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 1;
                    return 1;
                } else {
                    Base->CamerVerify[1] = 2;
                    return 2;
                }
            } else {
                Base->CamerVerify[2] = -1;
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 2;
                    return -2;
                } else {
                    Base->CamerVerify[1] = 1;
                    return -1;
                }
            }
        } else {
            static uint8_t i = 0;
            if (!i) {
                if (Base->CamerData[0] == K210Data) {
                    if (K210Site > 30) {
                        Base->CamerVerify[1] = 1;
                        return 1;
                    } else {
                        Base->CamerVerify[1] = 2;
                        return 2;
                    }
                } else {
                    Base->CamerVerify[3] = K210Data;
                    if (K210Site > 30) {
                        Base->CamerVerify[1] = 2;
                        return -2;
                    } else {
                        Base->CamerVerify[1] = 1;
                        return -1;
                    }
                }
            }
        }
        return -1;
    }
}

/// @brief 从摄像头验证数字
int8_t __Data_Get_from_Camer() {
    if (Base->SaveDataLock) {
        if (!K210Data) {
            return -1;
        }

        if (!Base->CamerVerify[0]) {
            if (Base->CamerData[0] == K210Data) {
                Base->CamerVerify[0] = K210Data;
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 1;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                    return 1;  // 右边
                } else {
                    Base->CamerVerify[1] = 2;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                    return 2;  // 左边
                }
            } else {
                Base->CamerVerify[2] = -1;
                if (Base->CamerVerify[1]) {
                    Base->CamerVerify[2] = 0;
                }
                return 3;
            }
        }
        return -1;
    }
}

/// @brief 近端病房模式
void __nearControl() {
    /*检查当前过路口情况*/
    __CrossManageNum();
    /*判断返回控制的触发条件*/
    if (Base->SiteLock == 1) { /*直线状态下*/
        if (Base->j == 1) {    /*假如已经转过一次弯*/
            static uint8_t j = 0;
            if (!j) {
                j                    = 1;
                Base->VerifyDataLock = 0; /*触发返回控制的条件*/
            }
        }
    }
    /*判断转向情况*/
    if (!Base->Back_sign) {                   /*非返回状态下*/
        if (Base->CamerData[0] == 1) {        /*若是数字1*/
            Base->Temp_RLContrl = 2;          /*暂时判断为转向右转*/
        } else if (Base->CamerData[0] == 2) { /*若是数字2*/
            Base->Temp_RLContrl = 1;          /*暂时判断为左转*/
        }
    } else {                                      /* 返回状态下*/
        Base->Temp_RLContrl = Base->CamerData[0]; /*转向状态相反*/
    }
    // 返回执行函数(主要是做转向的判断,如果条件触发)
    Base->Back();
    /*正式转向控制(根据临时的状态判断)*/
    __Dire_select(Base->Temp_RLContrl);

    /*判断启停*/
    if (Base->Back_sign == 3) {                           /*返回条件下*/
        if (Base->SiteLock == 2 || Base->SiteLock == 4) { /*如果是状态2:黑色,状态4:白色*/
            Base->RLControl = 4;                          /*直接触发制动*/
        }
    }
}

/// @brief 中端病房模式
void __minControl() {
    // 从摄像头验证数字
    // Base->Data_Get_from_Camer();
    static uint8_t temp_lock = 0;
    if (Base->j >= 1) {
        if ((Base->SiteLock != 3) && (!temp_lock)) {
            if (Base->CamerData[0] == K210Data) {
                Base->CamerVerify[0] = K210Data;
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 1;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                } else {
                    Base->CamerVerify[1] = 2;
                    Base->CamerVerify[2] = 0;
                    Base->SaveDataLock   = 2;
                }
            } else {
                Base->CamerVerify[2] = -1;
                if (Base->CamerVerify[1]) {
                    Base->CamerVerify[2] = 0;
                }
            }
        } else {
            temp_lock = 1;
        }
    }

    // 十字路口记录
    __CrossManageNum();
    // 临时转向控制
    __Temp_Dire_select();

    // 返回执行函数
    __Back();

    // 已返回状态下，触发遇黑色和白色暂停
    if (Base->Back_sign == 3) {
        if (Base->SiteLock == 2 || Base->SiteLock == 4) {
            Base->RLControl = 4;
        }
    }
    // 正式转向控制
    if (Base->j >= 1) {
        __Dire_select(Base->Temp_RLContrl);
    }
}

/// @brief 远端病房模式
void __farControl() {  // 确认是不是真的没有正确的数字

    // 从摄像头验证数字
    Base->Data_Get_from_Camer();

    // 十字路口记录
    __CrossManageNum();

    static uint8_t i = 0;
    static uint8_t j = 0;
    if (!i) {
        if (Base->SiteLock == 1) {
            i = 1;
        }
    }
    if ((Base->SiteLock == 5)||(Base->SiteLock == 6)) {
        if (i) {
            i = 2;
        }
    }
    if (Base->SiteLock == 1) {
        if (i == 2) {
            j++;
            i = 0;
        }
    }

    // 临时转向控制
    __Temp_Dire_select();

    // 返回执行函数
    __Back();

    // 已返回状态下，触发遇黑色和白色暂停
    if (Base->Back_sign == 4) {
        if (Base->SiteLock == 2 || Base->SiteLock == 4) {
            Base->RLControl = 4;
        }
    }

    // 正式转向控制
    __Dire_select(Base->Temp_RLContrl);

    if (Base->Back_sign == 3) {  // 单次运行，清除十字路口记录
        static uint8_t i = 0;
        if (!i) {
            Base->j = 0;
            i       = 1;
        }
    }

    if (Base->Back_sign == 3) {  // 经过路口两次,并且是返回状态
        MTurn_Strat(&j);
        if (Base->j == 2) {
            if (Base->Back_sign == 3) {
                Base->Key_Value = 2;
                //Base->Back_sign = 4;
            }
        }
    }
}

void NumExceptionHandler() {
    // 防止扫不到数字,出现转向异常

    static uint8_t i = 0;
    if (!i) {
        static uint8_t temp = 0;
        if ((Base->Back_sign != 3) && (Base->CamerVerify[0]) && (Base->j == 3)) {
            if (Base->CamerVerify[0] == K210Data) {
                if (K210Site > 30) {
                    Base->CamerVerify[1] = 1;
                } else {
                    Base->CamerVerify[1] = 2;
                }
            } else {
                temp = K210Site;
                if (Base->SiteLock == 3) {
                    Base->CamerVerify[2] = K210Data;
                    if (temp > 30) {
                        Base->CamerVerify[1] = 1;
                    } else {
                        Base->CamerVerify[1] = 2;
                    }
                }
            }
        } else if ((Base->Back_sign != 3) && (!Base->CamerVerify[0]) && (Base->j == 2)) {
            if (Base->CamerVerify[2] == 1) {
                if (Base->CamerData[0] == K210Data) {
                    Light_ON(2);
                    if (K210Site > 30) {
                        Base->CamerVerify[1] = 1;
                    } else {
                        Base->CamerVerify[1] = 2;
                    }
                } else {
                    temp = K210Site;
                    if (Base->SiteLock == 3) {
                        Base->CamerVerify[2] = K210Data;
                        if (temp > 30) {
                            Base->CamerVerify[1] = 1;
                        } else {
                            Base->CamerVerify[1] = 2;
                        }
                    }
                }
            }
        }
    }
}

// @brief 路径异常处理函数
void PathExceptionHandler() {
    __Data_Get_from_Camer();
    if (Base->SiteLock == 3) {
        if (Pet.Runstate == 0) {  // 由此进入异常
            static uint8_t a = 0;
            if (!a) {
                a++;
                Pet.Runstate = 1;
                Pet.SDL      = 1;
            }
        }
    } else if (Base->SiteLock == 1) {
        if (Pet.Runstate == 1) {  // 成功过弯，进入直线
            static uint8_t b = 0;
            if (!b) {
                b++;
                Pet.lock     = 1;
                Pet.Runstate = 2;
            }
        }
    }

    if (Pet.Runstate_2) {
        static uint8_t i = 0;
        if (!i) {
            if (Base->SiteLock == 1) {
                i = 1;
            }
        }
        if (Base->SiteLock == 5) {
            if (i) {
                i = 2;
            }
        }
        if (Base->SiteLock == 1) {
            if (i == 2) {
                Pet.PathNum++;
                i = 0;
            }
        }
    }
    if (Pet.Runstate == 1) {  // 过弯状态转向
        Pet.temp = 1;
    } else if (Pet.Runstate == 2) {  // 直线状态直走
        Pet.temp = 0;
        if ((Base->CamerVerify[0]) && (!Pet.Runstate_2)) {  // 假如扫到了数字就结束异常
            Base->Key_Value = 3;
        }
        if (Base->SiteLock == 3) {
            if (Base->CamerVerify[2] < 0) {  // 还是没扫到，进入调头状态
                Pet.Runstate = 3;
            }
        } else if ((Base->SiteLock == 5) && (Pet.Runstate_2 == 1)) {
            Pet.Runstate = 4;
        }
    } else if (Pet.Runstate == 3) {  // 调头状态
        Pet.temp = 3;
        if (Base->SiteLock == 1) {  // 调头完成，进入直线状态
            Pet.Runstate_2 = 1;
            Pet.Runstate   = 2;
        }
    } else if (Pet.Runstate == 4) {  // 遇到工口
        Pet.temp            = 0;
        PID_arg1.line_target = 200;
        if (Pet.PathNum == 2) {
            Pet.Runstate_2 = 2;
            Pet.Runstate   = 5;
        }
    } else if (Pet.Runstate == 5) {
        Pet.temp = 0;
        if (Base->SiteLock == 5) {
            Pet.Runstate = 4;
        }
        if (Base->SiteLock == 3) {
            Base->CamerVerify[2] = 1;
            Base->Key_Value      = 3;
        }
    }
    __Dire_select(Pet.temp);
}

/// @brief 转向状态记录函数
void MTurn_Strat(uint8_t *path) {
    static uint8_t i             = 0;
    static uint8_t Temp_SiteLock = 0;
    if (*path <= 2) {
        if (i) {                        // 如果工口转向已激活
            if (Base->SiteLock == 1) {  // 如果此时重新扫到中间线
                i = 0;                  // 暂时取消工口转向
                *path++;
            } else {
                Base->RLControl = Base->SiteLock;  // 如果此时没有扫到中间线，保持工口转向
            }
        }
        if ((Base->SiteLock == 5 || Base->SiteLock == 6) && (!i)) {  // 激活工口转向
            i = 1;
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

/// @brief 临时转向控制函数
uint8_t __Temp_Dire_select() {
    static uint8_t i = 0;
    if (!Base->Back_sign) {  // 小车宣布返回前，直接根据摄像头数据进行转向
        if (Base->CamerVerify[0]) {
            Base->Temp_RLContrl  = Base->CamerVerify[1];
            Base->VerifyDataLock = 0;
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
    if (Base->SiteLock == 3) {
        if (Temp) {
            Base->RLControl          = Temp;
            Base->MotorStrat_3_POINT = 1;  // 保证转向时不受白色背景停止的影响
        }
    } else if (Base->SiteLock == 1) {
        Base->RLControl          = 0;
        Base->MotorStrat_3_POINT = 0;
    }
}

/// @brief 返回处理函数
void __Back() {
    if (!Base->VerifyDataLock) {      // 当验证数据锁为0时，说明已经经过路口，此时激活返回控制函数
        if (Base->SiteLock == 4) {    // 假如扫到黑线
            if (!Base->Back_sign) {   // 且保证为第一次激活返回控制函数
                Base->Back_sign = 1;  // 返回控制进入第一阶段：暂停取药
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