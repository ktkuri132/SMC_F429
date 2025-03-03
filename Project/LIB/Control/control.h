#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>
#include <stdint.h>

int8_t Project_LIB_ControlStrat();
void control_near_Init();



typedef struct Control {

    uint8_t MotorStrat_1; //  电机启动最高优先级：电源控制，默认为1
                          //  默认插上跳线帽为标准功耗，开启电机，拔下跳线帽为低功耗，关闭电机
    uint8_t MotorStrat_2; //  电机启动第二优先级：药品放置，默认为0
                            //  默认放置药品就开启电机，拿走药品就关闭电机
    uint8_t MotorStrat_3_POINT;
    uint8_t MotorStrat_3; //  电机启动第三优先级：巡线识别，默认为0
                          //  默认巡线识别就开启电机，停止巡线识别就关闭电机
    uint8_t Key_Value;
    uint8_t CamerData[4];
    uint8_t CamerVerify[4];
    uint8_t SaveDataLock;
    uint8_t VerifyDataLock;
    uint8_t SiteLock;
    uint8_t RLControl;
    uint8_t Temp_RLContrl;
    int8_t Rvalue, Lvalue;
    uint8_t old_RLControl;
    uint8_t Back_sign;
    uint8_t Turn_const;
    uint8_t Turn_start;
    
    void (*Control_Init)();
    void (*Motor_Load)(int32_t leftMotor, int32_t RightMotor);
    int8_t (*Data_Save_from_Camer)();
    int8_t (*Data_Get_from_Camer)();
    void (*Dire_select)(uint8_t Temp);
    void (*ControlTask)();
    void (*Back)();
}ctrl;

void  __Dire_select(uint8_t Temp);
ctrl* Control_Struct_Inti();
int8_t __Data_Save_from_Camer();
void __ControlTask();
void __Back();

typedef struct Control_near {
    ctrl *Base;
    uint8_t Dire_Load_ENABLE;
    uint8_t Turn_Const;
    void (*nearControl)();
} nctrl;

void __nearControl();
nctrl *Near_Struct_Inti();

typedef struct Control_min {
    ctrl *Base;
    uint8_t Turn_const;
    uint8_t Dire_Load_ENABLE;
    uint8_t __StieLock[4];
    void (*minControl)();
    void (*Turn_Sign)();
    uint8_t (*Temp_Dire_select)();

} mctrl;

mctrl *Min_Struct_Inti();
void __minControl();
int8_t __Data_Get_from_Camer();
uint8_t __Temp_Dire_select();


typedef struct Control_far {

    ctrl Base;

    void (*Control_Init)();
    void (*Data_Save_from_Camer)();
    void (*Data_Get_from_Camer)();
    void (*Temp_Dire_select)();
    void (*ControlTask)();
} fctrl;

#endif // !_CONTROL_