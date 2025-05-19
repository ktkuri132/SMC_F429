#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>
#include <string.h>

#include "Project.h"
#include "usart/Serial.h"

extern ctrl *Base;
extern PID_arg PID_arg1;

uint8_t Get_RLcontrol(uint8_t i);

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

void Mid_Mode(uint8_t srlt, uint8_t srlt_2) {
    static uint8_t turn_e = 0;  /*设置停止标志位,防止小车一开始因为没有启动信号而停止*/
    Base->Temp_RLContrl = srlt;
    __CrossManageNum();
    // 正式转向控制
    if ((Base->j >= 1) && (Base->j <= 2)) {
        __Dire_select(Base->Temp_RLContrl);
        if (Base->Temp_RLContrl == 3) {  /*此时需要调头*/
            if ((Base->SiteLock == 2) || (Base->SiteLock == 4)) {   /*如果还是黑色或白色*/
                Base->RLControl = 3;    /*持续调头*/
                Base->MotorStrat_3_POINT = 1;       /*不受白色背景停止的影响*/
            } else {    /*否则,如果看到中心红线,则说明调头停止*/
                turn_e = 1;     /*打开停止标志位*/
                Base->RLControl = 0;    /*让车暂停*/
                Base->MotorStrat_3_POINT = 0;       /*重新打开白色背景限制*/
            }
        }
    } else if (Base->j >= 3) {
        PID_arg1.speed_target = 20;
        __Dire_select(0);
    }

    if((!srlt_2)&&(srlt == 3)&&(turn_e)){    // 触发调头结束条件:启动信号 1,调头信号 3,停止标志 0
        Project_BSP_LED_ON(1);
        Base->RLControl = 4;
    }

}

void Far_Mode(uint8_t srlt, uint8_t srlt_2){
    uint8_t Temp_RL = Get_RLcontrol(1);
    __CrossManageNum();
    if(Base->j>=2){ /*进入远端模式*/
        Base->Temp_RLContrl = srlt;
        __Dire_select(Base->Temp_RLContrl);
        if((Base->j >2)&&(Base->j <= 3)){   /*过第一个弯*/
            Base->Temp_RLContrl = srlt_2;   /*传输转向方向*/
            __Dire_select(Base->Temp_RLContrl);
        }
        if(Base->j >= 4){    /*过第二个弯*/
            Base->Temp_RLContrl = srlt_2;   /*传输转向方向*/
            __Dire_select(Base->Temp_RLContrl);
            if((Base->SiteLock == 4)||(Base->SiteLock == 2)){   /*此时过程中遇到黑色或白色*/
                Base->RLControl = 4;    /*触发停止*/
                                
            }
        }
    }
}
