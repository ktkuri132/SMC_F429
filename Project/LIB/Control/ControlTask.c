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
extern ctrl Base;


PID_arg PID_arg1 = {180, 20};

uint8_t Get_RLcontrol(uint8_t i) {
    uint8_t tmp;
    if(i == 1){
        tmp = OtherCar;
    } else if(i == 2){
        tmp = OtherCar_S2;
    } else if(i == 3){
        tmp = OtherCar_S3;
    } else if(i == 4){
        tmp = OtherCar_S4;
    } else if(i == 5){
        tmp = OtherCar_S5;
    } else {
        return 0;  // 错误
    }
    return tmp;  // 默认值
}

/**
 * @brief 控制任务选择函数
 * @note 根据模式选择不同的控制任务
 */
void __ControlTask() {
    static uint8_t Turn_sign      = 0;  // 转向信号
    static uint8_t Turn_sign_temp = 0;  // 临时转向信号
    /*获取小车1的数字,如果没有,没有就退出等待*/
    uint8_t srlt = Get_RLcontrol(1);
    uint8_t srlt_2 = Get_RLcontrol(2);
    if (!srlt) {
        return;
    }
    static uint8_t i = 0;
    if(!i){
        usart_send_string(UART4, "s7,1,e");
        i = 1;
    }
    
    switch (Base.Key_Value)
    {
    case 1 :{
        Mid_Mode(srlt, srlt_2);  // 中端控制
    }break;
    case 2:{
        Far_Mode(srlt, srlt_2);  // 远端控制
    }break;
    case 3:{
        Other_Mode();  // 其他模式
    }break;
    default:
        break;
    }

    /*根据之前的判断来确定速度,转向等情况*/
    Project_LIB_ControlTask(Base.RLControl);
}

/// @brief 控制任务
void Project_LIB_ControlTask(uint8_t rlControl) {
    static PID pidForLine;  // 创建PID结构体
    static PID pidforspeed;

    PID_TypeStructInit(&pidforspeed, 400, 10, 0,
                       22);  // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 10, 28, 0,
                       PID_arg1.line_target);  // 初始化寻线PID,目标值：中线坐标

    pidForLine.PID_Update1  = PID_forLine;
    pidforspeed.PID_Update1 = speedControl;

    pidForLine.PID_Update1(&pidForLine);
    pidforspeed.PID_Update1(&pidforspeed);

    if (rlControl == 2) {  // 左拐
        Base.Motor_Load(3200, 1000);
    } else if (rlControl == 1) {  // 右拐
        Base.Motor_Load(1000, 3200);
    } else if (rlControl == 3) {  // 调头
        Base.Motor_Load(-1400, 1400);
    } else if (rlControl == 4) {  // 停车
        Base.Motor_Load(0, 0);
    } else if (rlControl == 6) {
        if (Base.Key_Value == 3) {
            Project_BSP_LED_ON(1);
            Base.Motor_Load(1000, 5000);
        }
    } else if (rlControl == 5) {
        if (Base.Key_Value == 3) {
            Project_BSP_LED_ON(2);
            Base.Motor_Load(5000, 1000);
        }
    } else {
        Base.Motor_Load(pidforspeed.output + pidForLine.output,
                         pidforspeed.output - pidForLine.output);  // 装载到电机
    }
}
