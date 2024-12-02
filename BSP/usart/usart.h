#ifndef _USART_
#define _USART_

#include <stm32f4xx.h>

<<<<<<< HEAD
=======

// 选择接收协议
//#define _基本串口接受协议
//#define _正点原子串口协议
#define STDENOTE_USART
//#define _BIE串口协议


>>>>>>> 689952a31be5c63bcf8a7688bc60ebb66a60da67
void bsp_usart_1_inti(uint32_t baudrate);
void bsp_usart_2_inti(uint32_t baudrate);
void bsp_usart_3_inti(uint32_t baudrate);
void bsp_uart_4_inti(uint32_t baudrate);
void bsp_uart_5_inti(uint32_t baudrate);

<<<<<<< HEAD
=======
#ifdef STDUSART

#elif defined NORMAL_USART

#elif defined STDENOTE_USART

uint16_t USART_Deal(uint8_t point_note);

#elif defined BIE_USART

char* USART_Deal(uint8_t point_note);

#endif
>>>>>>> 689952a31be5c63bcf8a7688bc60ebb66a60da67


#endif