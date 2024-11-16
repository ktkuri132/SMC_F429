#include <stm32f4xx.h>
#include <gpio.h>
#include <stdio.h>
#include <usart.h>
#include <string.h>






// 串口重定向:实现_write函数,默认使用串口1
int _write(int file, char *ptr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        while ((USART1->SR & USART_SR_TXE) == 0)
            ;
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
        while ((USARTx->SR & USART_SR_TXE) == 0);
            
        USARTx->DR = data[i];
    }
}


/*
    串口1初始化
    PA9  TX
    PA10 RX
*/
void bsp_usart_1_inti(uint32_t baudrate)
{
    // 使能串口时钟和GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 配置串口引脚
    bsp_gpio_init(GPIOA, SYS_GPIO_PIN9, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOA, SYS_GPIO_PIN10, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);

    // 配置串口AF
    bsp_gpio_af_set(GPIOA, SYS_GPIO_PIN9, 7);
    bsp_gpio_af_set(GPIOA, SYS_GPIO_PIN10, 7);

    // 配置波特率:计算DIV，分成整数部分和小数部分，具体计算方法参考stm32f4开发手册
    float USART_DIV = (float)(SystemCoreClock/2)/(16*baudrate);
    uint16_t DIV_Mantissa = USART_DIV;                  // 波特率整数部分
    float DIV_Fraction = (USART_DIV-DIV_Mantissa)*16;   // 波特率小数部分
    DIV_Mantissa <<= 4;                                 // 将整数部分移动到正确的位置
    DIV_Mantissa += (uint16_t)DIV_Fraction;             // 加上小数部分

    USART1->BRR = DIV_Mantissa;
    // 使能串口
    USART1->CR1 |= USART_CR1_UE;
    // 数据位8位
    USART1->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    USART1->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    USART1->CR1 |= USART_CR1_RXNEIE;
    // 设置一个停止位
    USART1->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

    // 使能串口中断的部分在sys文件的NVIC_Configuration函数中
}


uint8_t USART_array[20][200];

uint8_t Data[20][200];

#ifdef _基本串口接受协议

// 串口1中断处理函数
void USART1_IRQHandler()
{
    if (USART1->SR & USART_SR_RXNE)
    {
        char data = USART1->DR;
        printf("data:%c\r\n", data);
    }
}

#elif defined _正点原子串口协议


#elif defined _STDE串口协议

uint8_t c;

uint8_t UART_Strat = 0;
uint8_t UART_End = 0;

uint8_t Res_Data_type = 0;    //数据格式检测标志位
uint8_t UART_DATA_TYPE = 0;    //真实数据格式

uint8_t Res_note = 0;  //数据节点
uint8_t Res_len = 0;    //接收数据的下标

uint8_t UART_NOTE = 0;  //本次数据节点
uint8_t UART_LEN = 0;   //本次接收数据的长度

uint8_t UART_NOTE_LEN[20];  //某次数据指定节点的数据长度


// 命令模式回调函数
void CMD_Callback()
{
    //处理命令
}

// 数据接收完成回调函数
void DR_Eevet_Callback()
{
    
    printf("hello,world!\r\n");
    
}

// 串口1中断处理函数：检测数据格式，接收数据
void USART1_IRQHandler()
{
    //判断是否是接收中断
    if (USART1->SR & USART_SR_RXNE)
    {
        c = USART1->DR;   //获取第一个字符
        if(c=='s')          //如果是开始字符
        {
            UART_Strat = 1;     //开始接收
            UART_End = 0;       //关闭结束标志
            Res_Data_type = 1;    //开始检测数据格式
            Res_len = 0;        //下标清零
            Res_note = 0;       //数据节点清零
            return;
        }
        else if(c=='e')     //如果是结束字符
        {
            UART_NOTE_LEN[Res_note] = Res_len;  //保存最后一次数据节点的长度

            UART_Strat = 0;     //重新开始
            UART_End = 1;       //打开结束标志
            UART_LEN = Res_len;   //保存本次数据长度
            UART_NOTE = Res_note+1;  //保存本次数据节点

            Res_note = 0;       //数据节点清零
            Res_len=0;          //下标清零
            memcpy(Data,USART_array,sizeof(USART_array));   //将数据拷贝到Data数组
            memset(USART_array,0,sizeof(USART_array));   //清空USART_array数组

            // 产生一个接收完成的事件(可选)
            DR_Eevet_Callback();

            return;
        }
        else if(c==',')
        {
            UART_NOTE_LEN[Res_note] = Res_len;  //保存本次数据节点的长度
            Res_note++;     //数据节点加1
            Res_len = 0;    //下标清零
        }
        else
        {
            if(Res_Data_type)     //先检测数据格式
            {
                switch (c)
                {
                    
                    // 检测数据格式
                    case '1':UART_DATA_TYPE=1;break;
                    // 其他数据格式......

                    // 检测：命令数据格式
                    case 'p':
                        UART_DATA_TYPE='p';
                        CMD_Callback();
                    break;

                    default:break;
                        
                }
                Res_Data_type=0;   //关闭数据格式检测
            }

            if(UART_Strat)      // 开始接收后，数组的第一个位置存储的是数据格式，读数据要从第二个位置开始
            {
                USART_array[Res_note][Res_len] = c;  //存储数据
                Res_len++;     //下标加1
            }
        }
        
    }
}


/// @brief 处理串口数据针对于数字
/// @param point_note 待处理的指定节点 
uint16_t USART_Deal(uint8_t point_note)
{
    //检查是否越界
    if(point_note > UART_NOTE)
    {
        return 0;
    }

    uint16_t sum = 0;
    uint8_t len = UART_NOTE_LEN[point_note];
    
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t temp_target = Data[point_note][i];
        // 判断临时目标数据的ascll码值
        if(!(temp_target >= '0' && temp_target <= '9'))     //如果不是数字字符
        {
            return temp_target;         //直接返回
        }
        
        sum = sum * 10 + (temp_target - '0');
    }

    return sum;
}

#elif defined _BIE串口协议

uint8_t c;

uint8_t UART_End = 1;

uint8_t Res_Data_type = 0;    //数据格式检测标志位
uint8_t UART_DATA_TYPE = 0;    //真实数据格式

uint8_t Res_note = 0;  //数据节点
uint8_t Res_len = 0;    //接收数据的下标

uint8_t UART_NOTE = 0;  //本次数据节点
uint8_t UART_LEN = 0;   //本次接收数据的长度

uint8_t UART_NOTE_LEN[20];  //某次数据指定节点的数据长度

int8_t Total_LEN=0;


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
void USART1_IRQHandler()
{
    //判断是否是接收中断
    if (USART1->SR & USART_SR_RXNE)
    {
        c = USART1->DR;   //获取第一个字符

        if(c==0x0D)     //如果是回车
        {
            UART_NOTE_LEN[Res_note] = Res_len;  //保存最后一次数据节点的长度

            UART_End = 1;       //打开结束标志
            UART_LEN = Res_len;   //保存本次数据长度
            UART_NOTE = Res_note+1;  //保存本次数据节点

            Res_note = 0;       //数据节点清零
            Res_len=0;          //下标清零
            memcpy(Data,USART_array,sizeof(USART_array));   //将数据拷贝到Data数组
            memset(USART_array,0,sizeof(USART_array));   //清空USART_array数组
            Total_LEN=0;
            // 产生一个接收完成的事件(可选)
            DR_Eevet_Callback();

            return;
        }
        else if(c==0x20)        // 检测到空格
        {
            USART_array[Res_note][Res_len] = c;  //存储数据

            UART_NOTE_LEN[Res_note] = Res_len;  //保存本次数据节点的长度
            Res_note++;     //数据节点加1
            Res_len = 0;    //下标清零
            Total_LEN++;
            // 输入回显
            USART1->DR = c;
        }
        else if(c==0x7F)  // 检测到backspace
        {
            if(Res_len!=0)      // 新的节点已经有字符了
            {
               USART_array[Res_note][Res_len-1] = 0;    // 上一次存储的字符换成0
               Res_len--;       // 下次存储字符的位置回退
            }
            else        // 这个新的节点还没有字符 回退键会将这个空格连带新产生的节点都删除
            {
                Res_note--;     // 节点的位置回退
                Res_len = UART_NOTE_LEN[Res_note]; // 上一个节点的最后一个位置取出来
            }

            if(Total_LEN>0)
            {
                // 回显删除字符
                USART1->DR = 0x08;  // 发送Backspace
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
                USART1->DR = ' ';   // 发送空格覆盖
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
                USART1->DR = 0x08;  // 再发送Backspace
                while (!(USART1->SR & USART_SR_TXE));  // 等待发送完成
            }
            Total_LEN--;
            if(Total_LEN<0)
            {
                Total_LEN=0;
            }
        }
        else
        {
            Total_LEN++;
            USART_array[Res_note][Res_len] = c;  //存储数据
            Res_len++;     //下标加1
            // 输入回显
            USART1->DR = c;
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
char* USART_Deal(uint8_t point_note)
{
    //检查是否越界
    if(point_note > UART_NOTE)
    {
        return 0;
    }

    int i;
    for(i=0;strcmp(Data[point_note],cmd[i]);i++)
    {
        if(i==19)
        {
            return "command not found!";
        }
    }

    return cmd[i];
}

#endif