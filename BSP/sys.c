#include "misc.h"
#include "stm32f4xx.h"
#include <errno.h>
#define SysTick_CLKSource_HCLK_Div8 ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK ((uint32_t)0x00000004)

/*
 * @brief 配置系统延迟函数
 */

/// @brief systick delay time in us
/// @param nus
void bsp_systick_delay_us(uint32_t nus) {
    SysTick->LOAD = nus * 21;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}

/// @brief systick delay time in ms
void bsp_systick_delay_ms(uint32_t nms) {
    uint32_t i;
    for (i = 0; i < nms; i++) {
        bsp_systick_delay_us(1000);
    }
}

/// @brief 配置系统中断优先级分组

/// @brief NVIC interrupt configuration
void NVIC_Configuration(void) {
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

#include <W25Q64/spiFlash.h>

/// @brief 将数据送进备份区
void PushBackup() {
#ifdef SPIFLASH_H

#endif
}

/// @brief 将备份数据
void ReadBackup() {}

/**
 *
 * 不使用标准库的重定向
 * @}
 */

int __attribute__((__weak__)) _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}

void __attribute__((__weak__)) _exit(int status) {
    _kill(status, -1);
    while (1) {
    } /* Make sure we hang here */
}

int __attribute__((__weak__)) _close(int file) {
    (void)file;
    return -1;
}

int __attribute__((__weak__)) _fstat(int file, struct stat *st) {
    (void)file;
    return 0;
}

int __attribute__((__weak__)) _isatty(int file) {
    (void)file;
    return 1;
}

int __attribute__((__weak__)) _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int __attribute__((__weak__)) _open(char *path, int flags, ...) {
    (void)path;
    (void)flags;
    /* Pretend like we always fail */
    return -1;
}

int __attribute__((__weak__)) _wait(int *status) {
    (void)status;
    return -1;
}

int __attribute__((__weak__)) _unlink(char *name) {
    (void)name;
    return -1;
}

int __attribute__((__weak__)) _times(struct tms *buf) {
    (void)buf;
    return -1;
}

int __attribute__((__weak__)) _stat(char *file, struct stat *st) {
    (void)file;
    return 0;
}

int __attribute__((__weak__)) _link(char *old, char *new) {
    (void)old;
    (void)new;
    return -1;
}

int __attribute__((__weak__)) _fork(void) { return -1; }

int __attribute__((__weak__)) _execve(char *name, char **argv, char **env) {
    (void)name;
    (void)argv;
    (void)env;
    return -1;
}


static uint8_t *__sbrk_heap_end = NULL;


void __attribute__((__weak__)) _sbrk(int incr) {
    extern uint8_t _end;             /* Symbol defined in the linker script */
    extern uint8_t _estack;          /* Symbol defined in the linker script */
    extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
    const uint32_t stack_limit =
        (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
    const uint8_t *max_heap = (uint8_t *)stack_limit;
    uint8_t *prev_heap_end;

    /* Initialize heap end at first call */
    if (NULL == __sbrk_heap_end) {
        __sbrk_heap_end = &_end;
    }

    /* Protect heap from growing into the reserved MSP stack */
    if (__sbrk_heap_end + incr > max_heap) {
        errno = ENOMEM;
        return (void *)-1;
    }

    prev_heap_end = __sbrk_heap_end;
    __sbrk_heap_end += incr;

    return (void *)prev_heap_end;
}

int __attribute__((__weak__)) _getpid(void) {
    (void)1;
    return 1;
}
