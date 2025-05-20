#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>
#include <stdint.h>

int8_t Project_LIB_ControlStrat();
void control_near_Init();

/**
 * @brief 路径控制结构体
 * 
 */
typedef struct {
    uint8_t CrossNum;   // 路口编号
    uint8_t CrossState;  // 路口状态
} CrossManage;

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
    uint8_t CamerData[4];   // 摄像头数据
    int8_t CamerVerify[4];  // 摄像头验证数据
    uint8_t SaveDataLock;   // 摄像头数据存储锁
    uint8_t VerifyDataLock; // 摄像头数据验证锁
    uint8_t SiteLock;       // 路口状态
    uint8_t RLControl;      // 正式方向控制:左转,右转,启停
    uint8_t Temp_RLContrl;  // 临时方向控制
    int8_t Rvalue, Lvalue;  // 编码器值
    uint8_t old_RLControl;  // 上次方向状态
    uint8_t Back_sign;      // 返回状态控制位
    uint8_t Turn_const;     //  记录转向次数(未使用)
    uint8_t Turn_gather;    //  记录转向次数(未使用)
    uint8_t j;           //  记录路口次数(十字路口)
    uint8_t i;           //  记录路口次数(工字路口)

    void (*Control_Init)();     /*控制初始化函数*/
    void (*Motor_Load)(int32_t leftMotor, int32_t RightMotor);      /*电机装载函数*/ 
    int8_t (*Data_Save_from_Camer)();       /*从摄像头获取数字*/
    int8_t (*Data_Get_from_Camer)();        /*从摄像头验证数字(二次获取)*/
    void (*Dire_select)(uint8_t Temp);      /*方向选择函数*/
    void (*ControlTask)();      /*任务模式控制函数*/
    void (*Back)();     /*返回控制函数*/
}ctrl;

void  __Dire_select(uint8_t Temp);
void Control_Struct_Inti();
int8_t __Data_Save_from_Camer();
void __ControlTask();
uint8_t __CrossManageNum();
void __Back();

/**
 * @brief 近端病房控制结构体
 * 
 */
typedef struct Control_near {
    /*继承主结构体*/
    ctrl *Base;
    uint8_t Turn_const;     // 记录转向次数(未使用)
    uint8_t Turn_start;     // 记录转向次数(未使用)
    
    void (*nearControl)();      /*近端控制函数*/
} nctrl;

void __nearControl();       /*近端控制初始化函数*/
nctrl *Near_Struct_Inti();

/**
 * @brief 中端病房控制结构体
 * 
 */
typedef struct Control_min {
    ctrl *Base;     /*继承控制结构体*/
    uint8_t Turn_const;     /*记录转向状态*/
    void (*minControl)();   /*中端控制函数*/
    void (*Turn_Sign)();    /*未使用*/
    uint8_t (*Temp_Dire_select)();  /*临时转向函数*/

} mctrl;


mctrl *Min_Struct_Inti();       /*终端控制函数*/
void __minControl();
int8_t __Data_Get_from_Camer();     /*中端:摄像头获取函数*/
int8_t Data_Get_mid();
uint8_t __Temp_Dire_select();   /*远端:转向状态记录函数*/

/**
 * @brief 远端病房控制结构体
 * 
 */
typedef struct Control_far {

    ctrl *Base;
    uint8_t (*Temp_Dire_select)();
    void (*farControl)();
} fctrl;

int8_t Data_Get_far();  /*远端:摄像头获取函数*/
fctrl *Far_Struct_Inti();   /*远端控制初始化函数*/
void __farControl();        /*远端控制函数*/

/**
 * @brief 远端病房路径异常处理结构体
 * 
 */
typedef struct PathException_Typedef {
    __IO uint8_t lock;
    __IO uint8_t temp;
    __IO uint8_t PathNum;
    __IO uint8_t SDL;
    __IO uint8_t Error;
    __IO uint8_t Runstate;
    __IO uint8_t Runstate_2;

}PET;

void NumExceptionHandler(); /*数字异常处理*/
PET *Pet_Struct_Inti();     /*路径异常处理初始化*/
void PathExceptionHandler();    /*路径异常处理*/

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
#endif // !_CONTROL_

