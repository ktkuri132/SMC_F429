#include "Serial.h"
#include <string.h>


// 串口重定向:实现_write函数,默认使用串口1
int _write(int file, char *ptr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        while ((USART1->SR & USART_SR_TXE) == 0);
        USART1->DR = ptr[i];
    }
    return len;
}

// 通用串口发送函数
void usart_send_string(USART_TypeDef *USARTx ,uint8_t *data)
{
    int i = 0;
    for (i = 0; data[i]!=0 ; i++)
    {
        USARTx->DR = data[i];
        while ((USARTx->SR & USART_SR_TXE) == 0);  
    }
}



#ifdef Basic_Serial

#elif defined ATOM_Serial       // 正点原子串口协议,用于兼容性交互

#elif defined STDE_Serial       // st,d,..e 串口协议,用于常规通信

// 命令模式回调函数
__attribute__((weak)) void CMD_Callback() 
{
    //处理命令
}

// 数据接收完成回调函数
__attribute__((weak)) void DR_Eevet_Callback()
{
    //处理命令
}

// 初始化函数，模拟类的构造函数
void Stde_DataTypeDef_Init(Stde_DataTypeDef *data)
{
    data->CMD_Callback = CMD_Callback;
    data->DR_Eevet_Callback = DR_Eevet_Callback;
    data->c = 0;
    data->UART_Strat = 0;
    data->UART_End = 0;
    data->Res_Data_type = 0;
    data->UART_DATA_TYPE = 0;
    data->Res_note = 0;
    data->Res_len = 0;
    data->UART_NOTE = 0;
    data->UART_LEN = 0;
    memset(data->UART_NOTE_LEN, 0, sizeof(data->UART_NOTE_LEN));
    memset(data->USART_array, 0, sizeof(data->USART_array));
    memset(data->Data, 0, sizeof(data->Data));
}

/// @brief STDE串口协议处理函数
void STDE_UART(USART_TypeDef *USARTx,Stde_DataTypeDef* DataTypeStruct)
{
    DataTypeStruct->c = USARTx->DR;   //获取第一个字符
    // printf("%c",DataTypeStruct->c);
    if(DataTypeStruct->c=='s')          //如果是开始字符
    {
        DataTypeStruct->UART_Strat = 1;     //开始接收
        DataTypeStruct->UART_End = 0;       //关闭结束标志
        DataTypeStruct->Res_Data_type = 1;    //开始检测数据格式
        DataTypeStruct->Res_len = 0;        //下标清零
        DataTypeStruct->Res_note = 0;       //数据节点清零
        return 0;
    }
    else if(DataTypeStruct->c=='e')     //如果是结束字符
    {
        DataTypeStruct->UART_NOTE_LEN[DataTypeStruct->Res_note] = DataTypeStruct->Res_len;  //保存最后一次数据节点的长度

        DataTypeStruct->UART_Strat = 0;     //重新开始
        DataTypeStruct->UART_End = 1;       //打开结束标志
        DataTypeStruct->UART_LEN = DataTypeStruct->Res_len;   //保存本次数据长度
        DataTypeStruct->UART_NOTE = DataTypeStruct->Res_note+1;  //保存本次数据节点

        DataTypeStruct->Res_note = 0;       //数据节点清零
        DataTypeStruct->Res_len=0;          //下标清零
        memcpy(DataTypeStruct->Data,DataTypeStruct->USART_array,sizeof(DataTypeStruct->USART_array));   //将数据拷贝到Data数组
        memset(DataTypeStruct->USART_array,0,sizeof(DataTypeStruct->USART_array));   //清空USART_array数组

        // 数据帧结束，产生回调事件
        DataTypeStruct->DR_Eevet_Callback();

        return 0;
    }
    else if(DataTypeStruct->c==',')
    {
        DataTypeStruct->UART_NOTE_LEN[DataTypeStruct->Res_note] = DataTypeStruct->Res_len;  //保存本次数据节点的长度
        DataTypeStruct->Res_note++;     //数据节点加1
        DataTypeStruct->Res_len = 0;    //下标清零
    }
    else
    {
        if(DataTypeStruct->Res_Data_type)     //先检测数据格式
        {
            switch (DataTypeStruct->c)
            {
                
                // 检测数据格式
                case '1':DataTypeStruct->UART_DATA_TYPE=1;break;
                // 其他数据格式......
                case '2':DataTypeStruct->UART_DATA_TYPE=2;break;
                // 检测：命令数据格式
                case 'p':
                    DataTypeStruct->UART_DATA_TYPE='p';
                    DataTypeStruct->CMD_Callback();
                break;

                default:break;
                    
            }
            DataTypeStruct->Res_Data_type=0;   //关闭数据格式检测
        }

        if(DataTypeStruct->UART_Strat)      // 开始接收后，数组的第一个位置存储的是数据格式，读数据要从第二个位置开始
        {
            DataTypeStruct->USART_array[DataTypeStruct->Res_note][DataTypeStruct->Res_len] = DataTypeStruct->c;  //存储数据
            DataTypeStruct->Res_len++;     //下标加1
        }
    }
}

/// @brief 处理串口数据针对于数字
/// @param point_note 待处理的指定节点 
uint16_t USART_Deal(Stde_DataTypeDef* DataTypeStruct,uint8_t point_note)
{
    //检查是否越界
    if(point_note > DataTypeStruct->UART_NOTE)
    {
        return 0;
    }

    uint16_t sum = 0;
    uint8_t len = DataTypeStruct->UART_NOTE_LEN[point_note];
    
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t temp_target = DataTypeStruct->Data[point_note][i];
        // 判断临时目标数据的ascll码值
        if(!(temp_target >= '0' && temp_target <= '9'))     //如果不是数字字符
        {
            return temp_target;         //直接返回
        }
        
        sum = sum * 10 + (temp_target - '0');
    }

    return sum;
}


#elif defined BIE_Serial        // 专用于模拟PC交互shell脚本协议

// 命令模式回调函数
void CMD_Callback()
{
    //处理命令
}

// 数据接收完成回调函数
void DR_Eevet_Callback()
{
    if(USART_Deal(0)==NULL)
    {
        printf("\nroot@stm32:");
    }
    else 
    {
        printf("\n%s\nroot@stm32:",USART_Deal(0));
    }
    
    fflush(stdout);
}

// 串口1中断处理函数：检测数据格式，接收数据
void BIE_UART(USART_TypeDef * USARTx,Bie_ShellTypeDef* ShellTypeStruct)
{
    //判断是否是接收中断
    if (USART1->SR & USART_SR_RXNE)
    {
        ShellTypeStruct->c = USART1->DR;   //获取第一个字符

        if(ShellTypeStruct->c==0x0D)     //如果是回车
        {
            ShellTypeStruct->UART_NOTE_LEN[ShellTypeStruct->Res_note] = ShellTypeStruct->Res_len;  //保存最后一次数据节点的长度

            ShellTypeStruct->UART_End = 1;       //打开结束标志
            ShellTypeStruct->UART_LEN = ShellTypeStruct->Res_len;   //保存本次数据长度
            ShellTypeStruct->UART_NOTE = ShellTypeStruct->Res_note+1;  //保存本次数据节点

            ShellTypeStruct->Res_note = 0;       //数据节点清零
            ShellTypeStruct->Res_len=0;          //下标清零
            memcpy(ShellTypeStruct->Data,ShellTypeStruct->USART_array,sizeof(ShellTypeStruct->USART_array));   //将数据拷贝到Data数组
            memset(ShellTypeStruct->USART_array,0,sizeof(ShellTypeStruct->USART_array));   //清空USART_array数组
            ShellTypeStruct->Total_LEN=0;
            // 产生一个接收完成的事件(可选)
            DR_Eevet_Callback();

            return;
        }
        else if(ShellTypeStruct->c==0x20)        // 检测到空格
        {
            ShellTypeStruct->USART_array[ShellTypeStruct->Res_note][ShellTypeStruct->Res_len] = ShellTypeStruct->c;  //存储数据

            ShellTypeStruct->UART_NOTE_LEN[ShellTypeStruct->Res_note] = ShellTypeStruct->Res_len;  //保存本次数据节点的长度
            ShellTypeStruct->Res_note++;     //数据节点加1
            ShellTypeStruct->Res_len = 0;    //下标清零
            ShellTypeStruct->Total_LEN++;
            // 输入回显
            USART1->DR = ShellTypeStruct->c;
        }
        else if(ShellTypeStruct->c==0x7F)  // 检测到backspace
        {
            if(ShellTypeStruct->Res_len!=0)      // 新的节点已经有字符了
            {
               ShellTypeStruct->USART_array[ShellTypeStruct->Res_note][ShellTypeStruct->Res_len-1] = 0;    // 上一次存储的字符换成0
               ShellTypeStruct->Res_len--;       // 下次存储字符的位置回退
            }
            else        // 这个新的节点还没有字符 回退键会将这个空格连带新产生的节点都删除
            {
                ShellTypeStruct->Res_note--;     // 节点的位置回退
                ShellTypeStruct->Res_len = ShellTypeStruct->UART_NOTE_LEN[ShellTypeStruct->Res_note]; // 上一个节点的最后一个位置取出来
            }

            if(ShellTypeStruct->Total_LEN>0)
            {
                // 回显删除字符
                USART1->DR = 0x08;  // 发送Backspace
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
                USART1->DR = ' ';   // 发送空格覆盖
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
                USART1->DR = 0x08;  // 再发送Backspace
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
            }
            ShellTypeStruct->Total_LEN--;
            if(ShellTypeStruct->Total_LEN<0)
            {
                ShellTypeStruct->Total_LEN=0;
            }
        }
        else
        {
            ShellTypeStruct->Total_LEN++;
            ShellTypeStruct->USART_array[ShellTypeStruct->Res_note][ShellTypeStruct->Res_len] = ShellTypeStruct->c;  //存储数据
            ShellTypeStruct->Res_len++;     //下标加1
            // 输入回显
            USART1->DR = ShellTypeStruct->c;
        }
    }
}

// 待添加的命令
char* cmd[20] = 
{
    "hello","reboot","poweroff"
};


/// @brief 处理串口发送的指令
/// @param point_note 待处理的节点
/// @return 字符串指针
uint8_t Shell_Deal(uint8_t point_note,Bie_ShellTypeDef* ShellTypeStruct)
{
    //检查是否越界
    if(point_note > ShellTypeStruct->UART_NOTE)
    {
        return 0;
    }

    uint8_t i;
    for(i=0;strcmp(ShellTypeStruct->Data[point_note],cmd[i]);i++)
    {
        if(i==19)
        {
            return "command not found!";
        }
    }

    return i;
}


#endif