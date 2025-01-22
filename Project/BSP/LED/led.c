#include "Project.h"    // include the project header file

/// @brief LED --> C4,C5
void Project_BSP_LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    Project_BSP_LED_OFF(0);
    Project_BSP_LED_OFF(1);

}

void Project_BSP_LED_ON(uint8_t LED)
{
    if(LED == 0)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_4);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_5);
    }
}

void Project_BSP_LED_OFF(uint8_t LED)
{
    if(LED == 0)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_4);
    }
    else
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_5);
    }
}

void Project_BSP_Buzzer_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Project_BSP_Buzzer_ON()
{
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void Project_BSP_Buzzer_OFF()
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}   

