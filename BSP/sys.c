#include "stm32f4xx.h"
#include "misc.h"

#define SysTick_CLKSource_HCLK_Div8    ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)

void bsp_systick_delay_us(uint32_t nus);
void bsp_systick_delay_ms(uint32_t nms);

/// @brief systick delay time in us
/// @param nus 
void bsp_systick_delay_us(uint32_t nus)
{
    SysTick->LOAD = nus * 21; 
    SysTick->VAL = 0x00; 
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
    {
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
    SysTick->VAL = 0X00;
}

/// @brief systick delay time in ms
void bsp_systick_delay_ms(uint32_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
    {
        bsp_systick_delay_us(1000);
    }
}

/// @brief NVIC interrupt configuration 
void NVIC_Configuration(void)
{

    // 设置中断优先级分组为 4 位抢占优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置 USART1 中断优先级 调试信息
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 USART2 中断优先级 OpenMV摄像头
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 USART3 中断优先级 数字识别摄像头
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 UART4 中断优先级 无线串口
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 UART5 中断优先级 蓝牙串口
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置定时器1 中断优先级 PID控制的定时器
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}


/**
 * 
 * 这些几把库函数老是报警告，定义它们就不报警告了
  * @}
  */

void _exit() {

}

void _read_r()
{

}


void _lseek_r()
{

}

void _kill_r()
{

}

void _isatty_r()
{

}

void _getpid_r()
{

}

void _fstat_r()
{

}

void _close_r()
{

}

void _write_r()
{

}

void _sbrk()
{

}

void _open_r()
{

}







