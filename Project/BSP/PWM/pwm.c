#include <Project.h>

/// @brief 初始化TIM2为PWM输出:PA0,PA1,最大PWM设置为9000
void Project_BSP_PWM_TIM2_Init()
{
    //使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//初始化GPIOA
	bsp_gpio_init(GPIOA,SYS_GPIO_PIN0|SYS_GPIO_PIN1,SYS_GPIO_MODE_AF,SYS_GPIO_OTYPE_PP,SYS_GPIO_SPEED_HIGH,SYS_GPIO_PUPD_NONE);\
	//GPIOA复用为TIM2
	bsp_gpio_af_set(GPIOA,SYS_GPIO_PIN0,1);
	bsp_gpio_af_set(GPIOA,SYS_GPIO_PIN1,1);
	//初始化TIM2  Fpwm = 180/2MHZ->90MHZ / ((arr+1)*(psc+1))(单位：Hz)
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 9000-1; //计数器自动重装载值	arr
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //时钟预分频数 	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//初始化TIM2输出比较
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 4500; //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性高

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	//使能TIM2在PWM模式下输出
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	//使能TIM2
	TIM_Cmd(TIM2, ENABLE);
	
}


// @brief 设置TIM4_CH12_12_13--TIM15_CH12_H10_11的编码器模式
void Project_BSP_ENcoding_Init(uint16_t duty1,uint16_t duty2)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOH, ENABLE);
	//初始化GPIO
	bsp_gpio_init(GPIOD,SYS_GPIO_PIN12|SYS_GPIO_PIN13,SYS_GPIO_MODE_AF,SYS_GPIO_OTYPE_PP,SYS_GPIO_SPEED_HIGH,SYS_GPIO_PUPD_NONE);
	bsp_gpio_init(GPIOH,SYS_GPIO_PIN10|SYS_GPIO_PIN11,SYS_GPIO_MODE_AF,SYS_GPIO_OTYPE_PP,SYS_GPIO_SPEED_HIGH,SYS_GPIO_PUPD_NONE);

	bsp_gpio_af_set(GPIOD,SYS_GPIO_PIN12,2);
	bsp_gpio_af_set(GPIOD,SYS_GPIO_PIN13,2);

	bsp_gpio_af_set(GPIOH,SYS_GPIO_PIN10,9);
	bsp_gpio_af_set(GPIOH,SYS_GPIO_PIN11,9);

	//初始化TIM4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //计数器自动重装载值	arr
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //时钟预分频数 	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//初始化TIM4编码器模式
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM4, ENABLE);

	//初始化TIM5
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure2;
	TIM_TimeBaseStructure2.TIM_Period = 0xffff; //计数器自动重装载值	arr
	TIM_TimeBaseStructure2.TIM_Prescaler = 0; //时钟预分频数 	psc
	TIM_TimeBaseStructure2.TIM_ClockDivision = 0; //时钟分频
	TIM_TimeBaseStructure2.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure2);
	
	//初始化TIM5编码器模式
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM5, ENABLE);
}

