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
extern ctrl *Base;
extern nctrl *Near;
extern mctrl *Min;
extern fctrl *Far;
extern PET Pet;

PID_arg PID_arg1 = {180, 20};



uint8_t Get_RLcontrol() {
    uint8_t tmp = OtherCar;
    
    return tmp;  // 默认值
}

/**
 * @brief 控制任务选择函数
 * @note 根据模式选择不同的控制任务
 */
void __ControlTask() {
    /*获取摄像头的数字,如果没有,没有就退出等待*/
    int8_t dsfc_return = Base->Data_Save_from_Camer();
    if (dsfc_return < 0) {
        return;
    }
    
    /*双车通信*/
    uint8_t srlt = Get_RLcontrol();
    __Dire_select(srlt);
    /*根据之前的判断来确定速度,转向等情况*/
    Project_LIB_ControlTask(Base->RLControl);
}




/// @brief 控制任务
void Project_LIB_ControlTask(uint8_t rlControl) {
    static PID pidForLine;  // 创建PID结构体
    static PID pidforspeed;

    PID_TypeStructInit(&pidforspeed, 400, 10, 0, PID_arg1.speed_target);  // 为保持恒定速度不受电池电量影响
    PID_TypeStructInit(&pidForLine, 10,28 , 0, PID_arg1.line_target);  // 初始化寻线PID,目标值：中线坐标

    pidForLine.PID_Update1  = PID_forLine;
    pidforspeed.PID_Update1 = speedControl;

    pidForLine.PID_Update1(&pidForLine);
    pidforspeed.PID_Update1(&pidforspeed);

    if (rlControl == 2){  // 左拐
        Base->Motor_Load(3200, 1000);
    } else if (rlControl == 1){  // 右拐
        
        Base->Motor_Load(1000, 3200);
    } else if (rlControl == 3){  // 调头
        Base->Motor_Load(-1700, 1700);
    } else if (rlControl == 4) {  // 停车
        Base->Motor_Load(0, 0);
    } else if (rlControl == 6) {
        if(Base->Key_Value == 3){
            Project_BSP_LED_ON(1);
            Base->Motor_Load(1000, 5000);
        }
    } else if (rlControl == 5) {
        if(Base->Key_Value == 3){
            Project_BSP_LED_ON(2);
            Base->Motor_Load(5000, 1000);
        }
    } else {
        Base->Motor_Load(pidforspeed.output + pidForLine.output,
                         pidforspeed.output - pidForLine.output);  // 装载到电机
    }
}
