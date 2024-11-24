#include <stm32f4xx.h>


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
    // 设置中断优先级分组为 2 位抢占优先级和 2 位子优先级
    SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) | (0x3 << SCB_AIRCR_PRIGROUP_Pos);

    // 配置 USART1 中断优先级
    uint32_t prioritygroup = (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos;
    uint32_t priority = NVIC_EncodePriority(prioritygroup, 1, 1);
    NVIC->IP[USART1_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    NVIC->ISER[USART1_IRQn >> 5] = (1 << (USART1_IRQn & 0x1F)); // 使能中断

    // 配置外部中断 PH7 的优先级
    priority = NVIC_EncodePriority(prioritygroup, 2, 0);
    NVIC->IP[EXTI9_5_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    NVIC->ISER[EXTI9_5_IRQn >> 5] = (1 << (EXTI9_5_IRQn & 0x1F)); // 使能中断

    // 配置定时器2中断
    priority = NVIC_EncodePriority(prioritygroup, 2, 0);
    NVIC->IP[TIM2_IRQn] = (priority << 4) & 0xFF; // 设置优先级
    NVIC->ISER[TIM2_IRQn >> 5] = (1 << (TIM2_IRQn & 0x1F)); // 使能中断

}