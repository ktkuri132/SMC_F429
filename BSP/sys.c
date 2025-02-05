#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_rcc.h"

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

uint32_t GetGPIOPeriphClock(GPIO_TypeDef * GPIOx) {
    if (GPIOx == GPIOA) {
        return RCC_AHB1Periph_GPIOA;
    }
    else if (GPIOx == GPIOB) {
        return RCC_AHB1Periph_GPIOB;
    }
    else if (GPIOx == GPIOC) {
        return RCC_AHB1Periph_GPIOC;
    }
    else if (GPIOx == GPIOD) {
        return RCC_AHB1Periph_GPIOD;
    }
    else if (GPIOx == GPIOE) {
        return RCC_AHB1Periph_GPIOE;
    }
    else if (GPIOx == GPIOF) {
        return RCC_AHB1Periph_GPIOF;
    }
    else if (GPIOx == GPIOG) {
        return RCC_AHB1Periph_GPIOG;
    }
    else if (GPIOx == GPIOH) {
        return RCC_AHB1Periph_GPIOH;
    }
    else if (GPIOx == GPIOI) {
        return RCC_AHB1Periph_GPIOI;
    }
    else if (GPIOx == GPIOJ) {
        return RCC_AHB1Periph_GPIOJ;
    }
    else if (GPIOx == GPIOK) {
        return RCC_AHB1Periph_GPIOK;
    }
}


/**
 * 
 * 不使用标准库的重定向
  * @}
  */

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    return -1;
}

void _exit (int status)
{
    _kill(status, -1);
    while (1) {}    /* Make sure we hang here */
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = USART1->DR;
    }

    return len;
}

int _write(int file, char *ptr, int len)
{
    (void)file;
    int i = 0;
    for (i = 0; i < len; i++)
    {
        while ((USART1->SR & USART_SR_TXE) == 0);
        USART1->DR = ptr[i];
    }
    return len;
}

int _close(int file)
{
    (void)file;
    return -1;
}


int _fstat(int file, struct stat *st)
{
    (void)file;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _open(char *path, int flags, ...)
{
    (void)path;
    (void)flags;
    /* Pretend like we always fail */
    return -1;
}

int _wait(int *status)
{
    (void)status;
    return -1;
}

int _unlink(char *name)
{
    (void)name;
    return -1;
}

int _times(struct tms *buf)
{
    (void)buf;
    return -1;
}

int _stat(char *file, struct stat *st)
{
    (void)file;
    return 0;
}

int _link(char *old, char *new)
{
    (void)old;
    (void)new;
    return -1;
}

int _fork(void)
{
    return -1;
}

int _execve(char *name, char **argv, char **env)
{
    (void)name;
    (void)argv;
    (void)env;
    return -1;
}

int _sbrk(int incr) {
    (void)incr;
    return -1;
}

int _getpid(void) {
    (void)1;
    return 1;
}


