#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>
#include <stdint.h>

void Control_Struct_Inti();


/**
 * @brief 控制结构体
 * 
 */
typedef struct Control {

    uint8_t MotorStrat_1; //  电机启动最高优先级：电源控制，默认为1
                          //  默认插上跳线帽为标准功耗，开启电机，拔下跳线帽为低功耗，关闭电机
    uint8_t MotorStrat_2; //  电机启动第二优先级：药品放置，默认为0
                            //  默认放置药品就开启电机，拿走药品就关闭电机
    uint8_t MotorStrat_3_POINT;
    uint8_t MotorStrat_3; //  电机启动第三优先级：巡线识别，默认为0
                          //  默认巡线识别就开启电机，停止巡线识别就关闭电机
    uint8_t Key_Value;      // 模式选择
    uint8_t SiteLock;       // 路口状态
    uint8_t RLControl;      // 正式方向控制:左转,右转,启停
    uint8_t Temp_RLContrl;  // 临时方向控制
    int8_t Rvalue, Lvalue;  // 编码器值
    uint8_t Back_sign;      // 返回状态控制位
    uint8_t j;           //  记录路口次数(十字路口)
    uint8_t i;           //  记录路口次数(工字路口)

    void (*Control_Init)();     /*控制初始化函数*/
    void (*Motor_Load)(int32_t leftMotor, int32_t RightMotor);      /*电机装载函数*/ 
    void (*ControlTask)();      /*任务模式控制函数*/
}ctrl;

void __ControlTask();



/**
 * @brief PID控制参数结构体
 * 
 */
typedef struct{
    int16_t line_target;
    int16_t speed_target;
}PID_arg;


void Mid_Mode(uint8_t srlt, uint8_t srlt_2);
void Far_Mode(uint8_t srlt, uint8_t srlt_2);
void Other_Mode();
#endif // !_CONTROL_

