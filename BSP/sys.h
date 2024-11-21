#ifndef __NVIC_
#define __NVIC_

#include <stm32f4xx.h>
#include <gpio.h>

#include <stdio.h>

#include <usart.h>
#include <sys.h>
#include <OLED.h>

#include "sdram.h"

#ifdef __LCD_GUI_
#include "gt9xxx.h"
#include "ltdc.h"
#include "lcd.h"
#include "touch.h"
#endif

//#include <lcd_gui.h>

void NVIC_Configuration(void);
void bsp_systick_delay_us(uint32_t nus);
void bsp_systick_delay_ms(uint32_t nms);

#endif