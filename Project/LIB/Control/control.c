#include "Project.h"
#include "LIB/PID/pid.h"
#include <Project/LIB/Control/control.h>
#include <RTOSTaskConfig.h>
#include <comment/task.h>
#include <stdint.h>
#include <stdio.h>

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


ctrl *Control_Struct_Inti(){
    
    static ctrl base = {
        .MotorStrat_1 = 1,
        .MotorStrat_2 = 0,
        .MotorStrat_3 = 0,
        .ControlTask = __ControlTask,
        .Data_Save_from_Camer = __Data_Save_from_Camer,
       .Dire_select = __Dire_select,
    };
    return &base;
}

nctrl *Near_Struct_Inti(){
    static nctrl near = {
        .nearControl = __nearControl,
    };

    // 避免静态变量的初始化不是常量
    near.Base = Base;

    return &near;
}

int8_t __Data_Save_from_Camer()
{
    static TaskHandle_t *Task4_LEDPlayR_Handle;
    static TaskHandle_t *Task4_LEDPlayY_Handle;
    static uint8_t i = 0;
    if (!Base->SaveDataLock)
    {
        static uint8_t TempNum, TempSite;
        if (!K210Data)
        {
            if (Base->MotorStrat_2 || (Base->Key_Value == 1))
            { // 此时若电机被启动，直接强行锁定
                Base->SaveDataLock = 1;
                return i;
            }
            return -1;
        }
        if (TempNum == K210Data)
        {
            if (i == 2)
            { // 存入一个数字，选择性锁定存储
                if (Base->MotorStrat_2 || (Base->Key_Value == 1))
                { // 此时若电机被启动，直接强行锁定
                    Base->SaveDataLock = 1;
                }
            }
            return -2; //  上次和这次数据一样
        }
        TempNum  = K210Data; // 刷新临时数据
        TempSite = K210Site;
        if (TempNum)
        { // 如果识别到了数字
            if (!Base->CamerData[i])
            {                                // 如果缓冲数组目前为止为空
                Base->CamerData[i]     = TempNum;  // 填入数字
                Base->CamerData[i + 1] = TempSite; // 填入坐标
                if (!i)
                { // 返回0闪红灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Red_LED", 1024, 1, 10,
                                Task4_LEDPlayR_Handle);
                }
                else
                { // 返回1闪黄灯
                    xTaskCreate((TaskFunction_t)Task4_LEDPlay, "Yellow_LED", 1024, 2, 10,
                                Task4_LEDPlayY_Handle);
                }
                i += 2;
                if (i == 4)
                { // 存入两个数字，直接锁定数据存入
                    Base->SaveDataLock = 2;
                    return i;
                }
                return -1;
            }
        }
        else
        {              // 程序运行到这里，说明出现了扫描空挡
            return -1; // 返回3只会出现在，第一次出现扫描空挡的瞬间
        }
    }
    return i;
}

#define __Near Near->Base

void __nearControl(){

    if(__Near->CamerData[0] == 1){
        __Near->Temp_RLContrl = 2;
    }
    else if(__Near->CamerData[0] == 2){
        __Near->Temp_RLContrl = 1;
    }


    __Dire_select(__Near->Temp_RLContrl);
}

void  __Dire_select(uint8_t Temp)
{
    static uint8_t Turn_const = 0;
    static uint8_t Turn_start=0;
    static uint8_t Turn_end = 0;
    Base->old_RLControl = Base->RLControl;
    if (Base->SiteLock == 3)
    {
        if (Temp)
        {
            Base->RLControl = Temp;
        }
    }
    else
    {
        Base->RLControl = 0;
    }

    // 比较前后两次的转向选择是否一致，不一致说明转向状态发生了改变，第一次说明是开始转向，第二次说明转向结束
    if (Base->old_RLControl != Base->RLControl)
    {
        Turn_const++;

        if (Turn_const == 2)
        { // 变化方向满两次，验证数据锁次数-1,并且转向状态清零，变化次数清零
            Base->VerifyDataLock--;
            Turn_const    = 0;
            Base->Temp_RLContrl = 0;
            Base->CamerData[0] = 0;
        }
    }
}


void __Back(){
    
}
