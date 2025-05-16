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
extern PET *Pet;

PID_arg PID_arg1 = {180, 20};

/**
 * @brief 模式选择
 * @note 摄像头获取1,2为模式一,1,2以外的数字为模式二
 */
void Mode_chose() {
    if (Base->CamerData[0]) {
        if (Base->CamerData[0] == 1 || Base->CamerData[0] == 2) {
            Base->Key_Value = 1;
        } else {
            Base->Key_Value = 2;
        }
    }
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
    /*进入模式选择,只运行一次*/
    static uint8_t i = 0;
    if (!i) {
        Mode_chose();
        i++;
    }
    /*进入任务函数选择*/
    switch (Base->Key_Value) {
        case 1: {/*进入模式1*/
            static uint8_t i = 0;
            if (!i) {   /*初始化结构体,只运行一次*/
                Near = Near_Struct_Inti();  // 继承控制结构体
                i    = 1;
            }
            /*进入控制函数*/
            Near->nearControl();
        } break;
        /*以此类推......*/
        case 2: {
            static uint8_t i = 0;
            if (!i) {
                Min = Min_Struct_Inti();  // 继承控制结构体
                i   = 1;
            }
            Min->minControl();
            /*模式重新选择*/
            if ((Base->j == 2) && (!Base->Back_sign)) { /*非返回状态下,经过路口两次*/
                if(!Base->CamerVerify[0]){  /*假如此时还没有验证到数字*/
                    Base->Key_Value = 3;    /*判定为模式3*/
                }
            }
        } break;
        case 3: {
            static uint8_t i = 0;
            if (!i) {
                Far = Far_Struct_Inti();  // 继承控制结构体
                Pet = Pet_Struct_Inti();

                i   = 1;
            }
            Far->farControl();
            /*模式重新选择*/
            if (Base->SiteLock == 3) {  /*模式3中,遇到十字路口*/
                if ((!Base->CamerVerify[0]) && (!Pet->SDL)) {   /*此时还没有验证到数字,而且为第一次进入异常*/
                    Base->Key_Value = 4;    /*判定为模式4*/
                }
            }
        } break;
        case 4: {
            PathExceptionHandler();
        } break;
        default:
            break;
    }
    /*一次模式运行完毕,根据不同模式和路径情况调节速度*/
    if((Base->Key_Value == 2)||(Base->Key_Value == 1)){
        if(Base->j == 0){
            PID_arg1.speed_target = 26;
        } else if(Base->j == 1){
            PID_arg1.speed_target = 22;
        }
        if(Base->Back_sign == 3){
            PID_arg1.speed_target = 23;
        }
    } else if((Base->Key_Value == 3)||(Base->Key_Value == 4)){
        PID_arg1.speed_target = 21;
        if(Base->Back_sign == 3){
            PID_arg1.speed_target = 18;
        }
    }
    if(Base->Back_sign == 4){
        PID_arg1.speed_target = 32;
    }
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
