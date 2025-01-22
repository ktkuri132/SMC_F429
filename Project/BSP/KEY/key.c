#include "Project.h"

/// @brief 按键初始化：C6  C7 B13
void Project_BSP_KEY_Init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Project_BSP_GetKey()
{
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == 0)
    {
        return 1;
    }
    else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 0)
    {
        return 2;
    }
    else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}