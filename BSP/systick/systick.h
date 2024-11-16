#include <stm32f4xx.h>

#define SysTick_CLKSource_HCLK_Div8    ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)

void bsp_systick_delay_us(uint32_t nus);
void bsp_systick_delay_ms(uint32_t nms);
