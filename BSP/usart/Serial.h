#ifndef SERIAL_TOOL
#define SERIAL_TOOL

#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "usart.h"
void usart_send_string(USART_TypeDef *USARTx ,uint8_t *data);


// ANSI 转义序列定义

#define ANSI_CLEAR_SCREEN "\033[2J"                              // 清屏
#define ANSI_CURSOR_HOME "\033[H"                                // 将光标移动到左上角
#define ANSI_CURSOR_POSITION(row, col) "\033[" #row ";" #col "H" // 将光标移动到指定位置 (row, col)
#define ANSI_CURSOR_UP(n) "\033[" #n "A"                         // 将光标向上移动 n 行
#define ANSI_CURSOR_DOWN(n) "\033[" #n "B"                       // 将光标向下移动 n 行
#define ANSI_CURSOR_FORWARD(n) "\033[" #n "C"                    // 将光标向前移动 n 列
#define ANSI_CURSOR_BACKWARD(n) "\033[" #n "D"                   // 将光标向后移动 n 列
#define ANSI_SAVE_CURSOR "\033[s"                                // 保存光标位置
#define ANSI_RESTORE_CURSOR "\033[u"                             // 恢复光标位置
#define ANSI_ERASE_LINE "\033[K"                                 // 擦除从光标到行尾的内容
#define ANSI_TEXT_BOLD "\033[1m"                                 // 设置文本为粗体
#define ANSI_TEXT_UNDERLINE "\033[4m"                            // 设置文本为下划线
#define ANSI_TEXT_REVERSE "\033[7m"                              // 设置文本为反显
#define ANSI_TEXT_RESET "\033[0m"                                // 重置所有文本属性
#define ANSI_TEXT_COLOR(color) "\033[" #color "m"                // 设置文本颜色

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