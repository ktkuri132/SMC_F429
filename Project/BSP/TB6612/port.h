#ifndef _TB6612_PORT_
#define _TB6612_PORT_

#include <Project.h>

//PWM�ӿ�
//#define PWMA 22
//#define PWMB 20

//����ʹ�ܽӿ� use the GPIOA
#define AIN_2 2
#define AIN_1 3
#define BIN_1 4
#define BIN_2 5

//PWM���νӿ�
//#define MotorLeft PWMA
//#define MotorRight PWMB

//�������
#define LeftForward {PAout(AIN_1)=1;PAout(AIN_2)=0;}
#define LeftBackward {PAout(AIN_1)=0;PAout(AIN_2)=1;}
#define RightForward {PAout(BIN_1)=0;PAout(BIN_2)=1;}
#define RightBackward {PAout(BIN_1)=1;PAout(BIN_2)=0;}
#define AllForward {LeftForward;RightForward;}
#define AllBackward {LeftBackward;RightBackward;}
#define AllStop {PAout(AIN_1)=0;PAout(AIN_2)=0;PAout(BIN_1)=0;PAout(BIN_2)=0;}



#endif 