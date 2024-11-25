#ifndef _USART_
#define _USART_

#include <stm32f4xx.h>


// 选择接收协议
//#define _基本串口接受协议
//#define _正点原子串口协议
#define STDENOTE_USART
//#define _BIE串口协议


void bsp_usart_1_inti(uint32_t baudrate);

#ifdef STDUSART

#elif defined NORMAL_USART

#elif defined STDENOTE_USART

uint16_t USART_Deal(uint8_t point_note);

#elif defined BIE_USART

char* USART_Deal(uint8_t point_note);

#endif


#endif