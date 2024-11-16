#include <stm32f4xx.h>


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

}