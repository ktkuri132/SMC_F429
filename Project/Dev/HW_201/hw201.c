#include "hw201.h"

void Project_BSP_HW201_Init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    HW201_GET_PORT->PUPDR |= GPIO_PUPDR_PUPDR3_0;
    // bsp_gpio_init(HW201_GET_PORT, HW201_GET_PIN, SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);

    printf("初始化红外传感器\n");

}

uint8_t Project_BSP_HW201_Get()
{
    return HW201_GET_PORT->IDR & GPIO_IDR_IDR_3;
}
