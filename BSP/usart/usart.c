#include <stm32f4xx.h>
#include <gpio.h>
#include <stdio.h>
#include <usart.h>
#include <string.h>
#include <stm32f4xx_usart.h>
#include "Serial.h"





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

/*
    串口2初始化
    PD5  TX
    PD6 RX
*/
void bsp_usart_2_inti(uint32_t baudrate)
{
    // 使能串口时钟和GPIOA时钟
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // 配置串口引脚
    bsp_gpio_init(GPIOD, SYS_GPIO_PIN5, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOD, SYS_GPIO_PIN6, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);

    // 配置串口AF
    bsp_gpio_af_set(GPIOD, SYS_GPIO_PIN5, 7);
    bsp_gpio_af_set(GPIOD, SYS_GPIO_PIN6, 7);

    // 配置波特率:计算DIV，分成整数部分和小数部分，具体计算方法参考stm32f4开发手册
    float USART_DIV = (float)(SystemCoreClock/4)/(16*baudrate);
    uint16_t DIV_Mantissa = USART_DIV;                  // 波特率整数部分
    float DIV_Fraction = (USART_DIV-DIV_Mantissa)*16;   // 波特率小数部分
    DIV_Mantissa <<= 4;                                 // 将整数部分移动到正确的位置
    DIV_Mantissa += (uint16_t)DIV_Fraction;             // 加上小数部分

    USART2->BRR = DIV_Mantissa;
    // 使能串口
    USART2->CR1 |= USART_CR1_UE;
    // 数据位8位
    USART2->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    USART2->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    USART2->CR1 |= USART_CR1_RXNEIE;
    // 设置一个停止位
    USART2->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

/*
    串口3初始化
    PB10  TX
    PB11 RX
*/
void bsp_usart_3_inti(uint32_t baudrate)
{
    // 使能串口时钟和GPIOA时钟
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // 配置串口引脚
    bsp_gpio_init(GPIOB, SYS_GPIO_PIN10, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, SYS_GPIO_PIN11, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_NONE);

    // 配置串口AF
    bsp_gpio_af_set(GPIOB, SYS_GPIO_PIN10, 7);
    bsp_gpio_af_set(GPIOB, SYS_GPIO_PIN11, 7);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    // // 配置波特率:计算DIV，分成整数部分和小数部分，具体计算方法参考stm32f4开发手册
    float USART_DIV = (float)(SystemCoreClock/4)/(16*baudrate);
    uint16_t DIV_Mantissa = USART_DIV;                  // 波特率整数部分
    float DIV_Fraction = (USART_DIV-DIV_Mantissa)*16;   // 波特率小数部分
    DIV_Mantissa <<= 4;                                 // 将整数部分移动到正确的位置
    DIV_Mantissa += (uint16_t)DIV_Fraction;             // 加上小数部分

    USART3->BRR = DIV_Mantissa;
    // 使能串口
    USART3->CR1 |= USART_CR1_UE;
    // 数据位8位
    USART3->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    USART3->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    USART3->CR1 |= USART_CR1_RXNEIE;
    // 设置一个停止位
    USART3->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;

    

}

/*
    串口4初始化
    PC10  TX
    PC11 RX
*/
void bsp_uart_4_inti(uint32_t baudrate)
{
    // 使能串口时钟和GPIOA时钟
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // 配置串口引脚
    bsp_gpio_init(GPIOC, SYS_GPIO_PIN10, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOC, SYS_GPIO_PIN11, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);

    // 配置串口AF
    bsp_gpio_af_set(GPIOC, SYS_GPIO_PIN10, 8);
    bsp_gpio_af_set(GPIOC, SYS_GPIO_PIN11, 8);

    // 配置波特率:计算DIV，分成整数部分和小数部分，具体计算方法参考stm32f4开发手册
    float USART_DIV = (float)(SystemCoreClock/4)/(16*baudrate);
    uint16_t DIV_Mantissa = USART_DIV;                  // 波特率整数部分
    float DIV_Fraction = (USART_DIV-DIV_Mantissa)*16;   // 波特率小数部分
    DIV_Mantissa <<= 4;                                 // 将整数部分移动到正确的位置
    DIV_Mantissa += (uint16_t)DIV_Fraction;             // 加上小数部分

    UART4->BRR = DIV_Mantissa;
    // 使能串口
    UART4->CR1 |= USART_CR1_UE;
    // 数据位8位
    UART4->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    UART4->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    UART4->CR1 |= USART_CR1_RXNEIE;
    // 设置一个停止位
    UART4->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    UART4->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

/*
    串口5初始化
    PC12  TX
    PD2 RX
*/
void bsp_uart_5_inti(uint32_t baudrate)
{
    // 使能串口时钟和GPIOA时钟
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // 配置串口引脚
    bsp_gpio_init(GPIOC, SYS_GPIO_PIN12, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOD, SYS_GPIO_PIN2, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);

    // 配置串口AF
    bsp_gpio_af_set(GPIOC, SYS_GPIO_PIN12, 8);
    bsp_gpio_af_set(GPIOD, SYS_GPIO_PIN2, 8);

    // 配置波特率:计算DIV，分成整数部分和小数部分，具体计算方法参考stm32f4开发手册
    float USART_DIV = (float)(SystemCoreClock/4)/(16*baudrate);
    uint16_t DIV_Mantissa = USART_DIV;                  // 波特率整数部分
    float DIV_Fraction = (USART_DIV-DIV_Mantissa)*16;   // 波特率小数部分
    DIV_Mantissa <<= 4;                                 // 将整数部分移动到正确的位置
    DIV_Mantissa += (uint16_t)DIV_Fraction;             // 加上小数部分

    UART5->BRR = DIV_Mantissa;
    // 使能串口
    UART5->CR1 |= USART_CR1_UE;
    // 数据位8位
    UART5->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    UART5->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    UART5->CR1 |= USART_CR1_RXNEIE;
    // 设置一个停止位
    UART5->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    UART5->CR1 |= USART_CR1_TE | USART_CR1_RE;
}


Stde_DataTypeDef USART3_DataBuff;
Stde_DataTypeDef UART5_DataBuff;
Stde_DataTypeDef UART4_DataBuff;

void UART4_IRQHandler()
{
    if(UART4->SR & USART_SR_RXNE)
    {
        STDE_UART(UART4,&UART4_DataBuff);
    }
}

void USART3_IRQHandler()
{
    if(USART3->SR & USART_SR_RXNE)
    {
        STDE_UART(USART3,&USART3_DataBuff);
    }
}

void UART5_IRQHandler()
{
    if(UART5->SR & USART_SR_RXNE)
    {
        STDE_UART(UART5,&UART5_DataBuff);
    }
}