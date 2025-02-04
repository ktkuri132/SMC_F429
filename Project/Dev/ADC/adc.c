#include "Project.h"
#include "stm32f4xx_adc.h"

/// @brief ADC初始化
void Project_BSP_ADC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1,&ADC_InitStructure);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    // 初始化ADC通道
    ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_480Cycles);
    ADC_Cmd(ADC1,ENABLE);
    ADC_SoftwareStartConv(ADC1);
    
}

/// @brief 检测电池电压
/// @return 电量百分比
float Project_BSP_GetADC()
{
    float ADC_Value;
    ADC_Value = ADC_GetConversionValue(ADC1);
    ADC_Value = ADC_Value/4095*3.26;
    ADC_Value -= 2.316;
    ADC_Value = ADC_Value/0.83*100;
    ADC_Value = ADC_Value>100?100:ADC_Value;
    ADC_Value = ADC_Value<0?0:ADC_Value;
    return ADC_Value;
}


