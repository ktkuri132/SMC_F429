#include <lcd_gui.h>


void LCD_EXTI_Configuration(void)
{

    // 使能系统配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 使能GPIOH时钟
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;

    // 配置PH7为输入模式
    GPIOH->MODER &= ~(GPIO_MODER_MODER7);

    // 配置PH7为外部中断源
    SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI7);
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PH;

    // 配置中断触发方式，这里以上升沿触发为例
    EXTI->RTSR |= EXTI_RTSR_TR7;  // 上升沿触发
    EXTI->FTSR |= EXTI_FTSR_TR7; // 下降沿触发

    // 使能外部中断
    EXTI->IMR |= EXTI_IMR_MR7;
}