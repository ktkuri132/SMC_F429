#ifndef __HW201_H
#define __HW201_H

#include <Project.h>

#define HW201_GET_PORT GPIOB
#define HW201_GET_PIN  7

#define Project_BSP_HW201_Init()    do{RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;}while(0)
#define Project_BSP_HW201_Get       GPIOB->IDR & (1<<7)


#endif