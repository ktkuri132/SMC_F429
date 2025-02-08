#ifndef __NVIC_
#define __NVIC_

#include "stm32f4xx.h"
#include "gpio/gpio.h"
#include "sys.h"

#ifdef __LCD_GUI_
// #include "gt9xxx.h"
// #include "ltdc.h"
// #include "lcd.h"
// #include "touch.h"
#endif

#ifndef __STM32F4xx_GPIO_H
typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

#endif

void NVIC_Configuration(void);
void bsp_systick_delay_us(uint32_t nus);
void bsp_systick_delay_ms(uint32_t nms);

// #define delay_us(x) bsp_systick_delay_us(x)

#endif