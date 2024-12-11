#ifndef _TB6612_PORT_
#define _TB6612_PORT_


#include <Project.h>

//PWM接口
//#define PWMA 22
//#define PWMB 20

//方向使能接口 use the GPIOA
#define AIN_2 1
#define AIN_1 3
#define BIN_1 4
#define BIN_2 5

//PWM二次接口
//#define MotorLeft PWMA
//#define MotorRight PWMB

//方向控制
#define LeftForward {PAout(AIN_1)=1;PCout(AIN_2)=0;}
#define LeftBackward {PAout(AIN_1)=0;PCout(AIN_2)=1;}
#define RightForward {PAout(BIN_1)=1;PAout(BIN_2)=1;}
#define RightBackward {PAout(BIN_1)=0;PAout(BIN_2)=1;}
#define AllForward {LeftForward;RightForward;}
#define AllBackward {LeftBackward;RightBackward;}
#define AllStop {PAout(AIN_1)=0;PAout(AIN_2)=0;PAout(BIN_1)=0;PAout(BIN_2)=0;}




#endif 