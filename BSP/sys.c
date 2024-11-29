#include <stm32f4xx.h>
#include <misc.h>

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
    // 配置 USART1 中断优先级 调试信息
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 USART2 中断优先级 蓝牙串口
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 USART3 中断优先级 OpenMV摄像头
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 UART4 中断优先级 无线串口
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 UART5 中断优先级 V831摄像头
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置定时器5 中断优先级 PID控制的定时器
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // // 设置中断优先级分组为 2 位抢占优先级和 2 位子优先级
    // SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) | (0x3 << SCB_AIRCR_PRIGROUP_Pos);

    // // 配置 USART1 中断优先级   -------调试口中断
    // uint32_t prioritygroup = (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos;
    // uint32_t priority = NVIC_EncodePriority(prioritygroup, 3, 0);

    // NVIC->IP[USART1_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[USART1_IRQn >> 5] = (1 << (USART1_IRQn & 0x1F)); // 使能中断

    // // 配置 USART2 中断优先级   ------ 蓝牙串口中断
    // priority = NVIC_EncodePriority(prioritygroup, 2, 0);
    // NVIC->IP[USART2_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[USART2_IRQn >> 5] = (1 << (USART2_IRQn & 0x1F)); // 使能中断

    // // 配置 USART3 中断优先级   ------- OpenMV摄像头
    // priority = NVIC_EncodePriority(prioritygroup, 3, 0);
    // NVIC->IP[USART3_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[USART3_IRQn >> 5] = (1 << (USART3_IRQn & 0x1F)); // 使能中断

    // // 配置 UART4 中断优先级    -------- 无线串口
    // priority = NVIC_EncodePriority(prioritygroup, 2, 0);
    // NVIC->IP[UART4_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[UART4_IRQn >> 5] = (1 << (UART4_IRQn & 0x1F)); // 使能中断

    // // 配置 UART5 中断优先级    --------- V831摄像头
    // priority = NVIC_EncodePriority(prioritygroup, 1, 0);
    // NVIC->IP[UART5_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[UART5_IRQn >> 5] = (1 << (UART5_IRQn & 0x1F)); // 使能中断

    // // 配置外部中断 PH7 的优先级 ------ 触摸屏中断
    // priority = NVIC_EncodePriority(prioritygroup, 3, 0);
    // NVIC->IP[EXTI9_5_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[EXTI9_5_IRQn >> 5] = (1 << (EXTI9_5_IRQn & 0x1F)); // 使能中断

    // // 配置外部中断 A15 的优先级 ------ MPU6050中断
    // priority = NVIC_EncodePriority(prioritygroup, 3, 0);
    // NVIC->IP[EXTI15_10_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[EXTI15_10_IRQn >> 5] = (1 << (EXTI15_10_IRQn & 0x1F)); // 使能中断

    // 配置定时器2中断
    // priority = NVIC_EncodePriority(prioritygroup, 2, 0);
    // NVIC->IP[TIM2_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    // NVIC->ISER[TIM2_IRQn >> 5] = (1 << (TIM2_IRQn & 0x1F)); // 使能中断

}