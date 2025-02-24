#ifndef SERIAL_TOOL
#define SERIAL_TOOL

#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "usart.h"
void usart_send_string(USART_TypeDef *USARTx ,uint8_t *data);


#define STDE_Serial
// #define BIE_Serial


#ifdef Basic_Serial

#elif defined ATOM_Serial       // 正点原子串口协议,用于兼容性交互

#elif defined STDE_Serial       // St,d,..E 串口协议,用于常规通信

typedef struct
{
    void (*CMD_Callback)();        // 命令模式回调函数（可选）
    void (*DR_Eevet_Callback)();   // 结束帧回调函数（可选）
    void (*UART_DATA_TYPE_Callback)(struct Stde_DataTypeDef *DataTypeStruct);   // 数据类型回调函数（可选
    uint8_t c;                     // 单次数据

    uint8_t UART_Strat;            // 数据传输起始标志位
    uint8_t UART_End;              // 数据传输结束标志位

    uint8_t Res_Data_type;         //数据格式检测标志位
    uint8_t UART_DATA_TYPE;        //真实数据格式

    uint8_t Res_note;              //数据节点
    uint8_t Res_len;               //接收数据的下标
    uint8_t UART_NOTE;             //本次数据节点
    uint8_t UART_LEN;              //本次接收数据的长度

    uint8_t UART_NOTE_LEN[20];     //某次数据指定节点的数据长度

    uint8_t USART_array[10][10];   // 数据长度酌情调整

    uint8_t Data[10][10];          // 数据长度酌情调整

} Stde_DataTypeDef;

void Stde_DataTypeDef_Init(Stde_DataTypeDef *data);
uint8_t STDE_UART(USART_TypeDef * USARTx,Stde_DataTypeDef* DataTypeStruct);
uint16_t USART_Deal(Stde_DataTypeDef* DataTypeStruct,uint8_t point_note);


#elif defined BIE_Serial        // 专用于串口命令行终端交互协议

typedef struct 
{
    __IO uint8_t c;

    __IO uint8_t UART_End;

    __IO uint8_t Res_Data_type;    //数据格式检测标志位
    __IO uint8_t UART_DATA_TYPE;    //真实数据格式

    __IO uint8_t Res_note;  //数据节点
    __IO uint8_t Res_len;    //接收数据的下标

    __IO uint8_t UART_NOTE;  //本次数据节点
    __IO uint8_t UART_LEN;   //本次接收数据的长度

    __IO uint8_t UART_NOTE_LEN[20];  //某次数据指定节点的数据长度

    __IO int8_t Total_LEN;

    uint8_t USART_array[10][10];   // 数据长度酌情调整

    uint8_t Data[10][10];          // 数据长度酌情调整

}Bie_ShellTypeDef;


#endif

#endif