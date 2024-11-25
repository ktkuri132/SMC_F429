#include <hw201.h>

void Project_BSP_HW201_Init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    bsp_gpio_init(HW201_GET_PORT, HW201_GET_PIN, SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
}

uint8_t Project_BSP_HW201_Get()
{
    return bsp_gpio_pin_get(HW201_GET_PORT, HW201_GET_PIN);
}