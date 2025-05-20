#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>
#include <string.h>

#include "Project.h"
#include "usart/Serial.h"

extern ctrl Base;
extern PID_arg PID_arg1;

uint8_t Get_RLcontrol(uint8_t i);

void Control_Struct_Inti(){
    Base.MotorStrat_1 = 1; // 电机启动最高优先级：电源控制，默认为1
    Base.MotorStrat_2 = 0; // 电机启动第二优先级：药品放置，默认为0
    Base.MotorStrat_3 = 0; // 电机启动第三优先级：巡线识别，默认为0
    Base.MotorStrat_3_POINT = 0; // 白线限制,默认为开启限制
    Base.Key_Value     = 0; // 模式选择
    Base.RLControl     = 0; // 正式方向控制:左转,右转,启停
    Base.Temp_RLContrl = 0; // 临时方向控制
    Base.Back_sign     = 0; // 返回状态控制位
    Base.j            = 0; // 路口次数(十字路口)
    Base.i            = 0; // 路口次数(工字路口)
    Base.Motor_Load = forward_Motor_Load; // 小车启动条件改为：有药品启动
    Base.ControlTask = __ControlTask; // 任务模式控制函数
}

void Dire_select(uint8_t Temp) {
    if (Base.SiteLock == 3) {
        if (Temp) {
            Base.RLControl          = Temp;
            Base.MotorStrat_3_POINT = 1;  // 保证转向时不受白色背景停止的影响
        }
    } else if (Base.SiteLock == 1) {
        Base.RLControl          = 0;
        Base.MotorStrat_3_POINT = 0;
    }
}

uint8_t CrossManageNum(uint8_t mode) {
    if (mode == 1) {
        static uint8_t i = 0;
        if (!i) {
            if (Base.SiteLock == 1) {
                i = 1;
                return Base.j;
            }
        }
        if (Base.SiteLock == 3) {
            if (i) {
                i = 2;
                return Base.j;
            }
        }
        if (Base.SiteLock == 1) {
            if (i == 2) {
                Base.j++;
                i = 0;
                return Base.j;
            }
        }
    } else if(mode == 2) {
        static uint8_t i = 0;
        if (!i) {
            if (Base.SiteLock == 1) {
                i = 1;
                return Base.i;
            }
        }
        if ((Base.SiteLock == 5)||(Base.SiteLock == 6)) {
            if (i) {
                i = 2;
                return Base.i;
            }
        }
        if (Base.SiteLock == 1) {
            if (i == 2) {
                Base.i++;
                i = 0;
                return Base.i;
            }
        }
    }
}

/// @brief 返回处理函数
void Back(uint8_t *turn_e, uint8_t srlt, uint8_t srlt_2) {
    if (Base.Key_Value == 1) {
        if ((Base.j >= 1) && (Base.j <= 2)) {
            if (Base.Temp_RLContrl == 3) {                           /*此时需要调头*/
                if ((Base.SiteLock == 2) || (Base.SiteLock == 4)) { /*如果还是黑色或白色*/
                    Base.RLControl          = 3;                     /*持续调头*/
                    Base.MotorStrat_3_POINT = 1;                     /*不受白色背景停止的影响*/
                } else {                          /*否则,如果看到中心红线,则说明调头停止*/
                    *turn_e                  = 1; /*打开停止标志位,说明头已经调好了*/
                    Base.RLControl          = 0; /*让车启动*/
                    Base.MotorStrat_3_POINT = 0; /*重新打开白色背景限制*/
                }
            }
        }
        if ((!srlt_2) && (srlt == 3) &&
            (turn_e)) {  // 触发调头结束条件:启动信号 1,调头信号 3,停止标志 0
            Project_BSP_LED_ON(1);
            Base.Back_sign = 3;
            Base.RLControl = 4;
        }
    } else if (Base.Key_Value == 2) {
        if (!Base.MotorStrat_2) {               /*检测到药瓶被拿走*/
            Base.Motor_Load = back_Motor_Load;  // 小车启动条件改为：无药品启动
        }
        if (Base.Temp_RLContrl == 3) {                           /*此时需要调头*/
            if ((Base.SiteLock == 2) || (Base.SiteLock == 4)) { /*如果还是黑色或白色*/
                Base.RLControl          = 3;                     /*持续调头*/
                Base.MotorStrat_3_POINT = 1;                     /*不受白色背景停止的影响*/
            } else {                          /*否则,如果看到中心红线,则说明调头停止*/
                *turn_e                  = 1; /*打开停止标志位*/
                Base.RLControl          = 0; /*让车启动*/
                Base.MotorStrat_3_POINT = 0; /*重新打开白色背景限制*/
            }
        }
        if ((!srlt_2) && (srlt == 3) &&
            (turn_e)) {  // 触发调头结束条件:启动信号 1,调头信号 3,停止标志 0
            Project_BSP_LED_ON(2);
            Base.Back_sign = 3;
            Base.RLControl = 4;
        }
    }
}

/// @brief 转向状态记录函数
void MTurn_Strat(uint8_t path) {
    static uint8_t i             = 0;
    static uint8_t Temp_SiteLock = 0;
    if (path <= 2) {
        if (i) {                        // 如果工口转向已激活
            if (Base.SiteLock == 1) {  // 如果此时重新扫到中间线
                i = 0;                  // 暂时取消工口转向
            } else {
                Base.RLControl = Base.SiteLock;  // 如果此时没有扫到中间线，保持工口转向
            }
        }
        if ((Base.SiteLock == 5 || Base.SiteLock == 6) && (!i)) {  // 激活工口转向
            i = 1;
        }
    }
}

void Mid_Mode(uint8_t srlt, uint8_t srlt_2) {
    static uint8_t turn_e = 0; /*设置停止标志位,防止小车一开始因为没有启动信号而停止*/
    Base.Temp_RLContrl   = srlt;
    CrossManageNum(1);
    // 正式转向控制
    if ((Base.j >= 1) && (Base.j <= 2)) {
        Dire_select(Base.Temp_RLContrl);
    } else if (Base.j >= 3) {
        PID_arg1.speed_target = 20;
        Dire_select(0);
    }
    Back(&turn_e, srlt, srlt_2); /*返回处理函数*/
}

void Far_Mode(uint8_t srlt, uint8_t srlt_2) {
    static uint8_t turn_e = 0; /*设置停止标志位,防止小车一开始因为没有启动信号而停止*/
    static uint8_t i      = 0; /*运行控制标志位*/
    CrossManageNum(1);
    if (Base.j >= 2) { /*进入远端模式*/
        Base.Temp_RLContrl = srlt;
        Dire_select(Base.Temp_RLContrl);
        if ((Base.j > 2) && (Base.j <= 3)) { /*过第一个弯*/
            Base.Temp_RLContrl = srlt_2;      /*传输转向方向*/
            Dire_select(Base.Temp_RLContrl);
        }
        if (Base.j >= 4) {               /*过第二个弯*/
            Base.Temp_RLContrl = srlt_2; /*传输转向方向*/
            Dire_select(Base.Temp_RLContrl);
            /*此时,第一阶段:我接受3,0让我调头,第二阶段我接受3,1让我启动返回*/
            Back(&turn_e, srlt, srlt_2); /*返回处理函数*/
            if ((srlt == 3) && (turn_e) &&
                (srlt_2)) { /*如果我接受到调头信号,并且我已经调头完成,并且头车已经回到药房*/
                CrossManageNum(2);  /*记录工字路口*/
                MTurn_Strat(Base.i); /*工口转向*/
            }
        }
    }
}
