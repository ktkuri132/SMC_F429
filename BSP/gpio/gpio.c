#include "stm32f4xx.h"


/**
 * @brief       GPIO通用设置
 * @param       GPIOx: GPIOA~GPIOI, GPIO指针
 * @param       pin: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 *
 * @param       mode: 0~3; 模式选择, 设置如下:
 *   @arg       SYS_GPIO_MODE_IN,  0, 输入模式(系统复位默认状态)
 *   @arg       SYS_GPIO_MODE_OUT, 1, 输出模式
 *   @arg       SYS_GPIO_MODE_AF,  2, 复用功能模式
 *   @arg       SYS_GPIO_MODE_AIN, 3, 模拟输入模式
 *
 * @param       otype: 0 / 1; 输出类型选择, 设置如下:
 *   @arg       SYS_GPIO_OTYPE_PP, 0, 推挽输出
 *   @arg       SYS_GPIO_OTYPE_OD, 1, 开漏输出
 *
 * @param       ospeed: 0~3; 输出速度, 设置如下:
 *   @arg       SYS_GPIO_SPEED_LOW,  0, 低速
 *   @arg       SYS_GPIO_SPEED_MID,  1, 中速
 *   @arg       SYS_GPIO_SPEED_FAST, 2, 快速
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, 高速
 *
 * @param       pupd: 0~3: 上下拉设置, 设置如下:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, 不带上下拉
 *   @arg       SYS_GPIO_PUPD_PU,   1, 上拉
 *   @arg       SYS_GPIO_PUPD_PD,   2, 下拉
 *   @arg       SYS_GPIO_PUPD_RES,  3, 保留
 *
 * @note:       注意: 在输入模式(普通输入/模拟输入)下, OTYPE和OSPEED参数无效!!
 * @retval      无
 */
void bsp_gpio_init(GPIO_TypeDef *GPIOx, uint32_t PINx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = PINx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            GPIOx->MODER &= ~(3 << (pinpos * 2)); /* 先清除原来的设置 */
            GPIOx->MODER |= mode << (pinpos * 2); /* 设置新的模式 */

            if ((mode == 0X01) || (mode == 0X02))   /* 如果是输出模式/复用功能模式 */
            {
                GPIOx->OSPEEDR &= ~(3 << (pinpos * 2));       /* 清除原来的设置 */
                GPIOx->OSPEEDR |= (ospeed << (pinpos * 2));   /* 设置新的速度值 */
                GPIOx->OTYPER &= ~(1 << pinpos) ;             /* 清除原来的设置 */
                GPIOx->OTYPER |= otype << pinpos;             /* 设置新的输出模式 */
            }

            GPIOx->PUPDR &= ~(3 << (pinpos * 2)); /* 先清除原来的设置 */
            GPIOx->PUPDR |= pupd << (pinpos * 2); /* 设置新的上下拉 */
        }
    }
}

/**
 * @brief       GPIO复用功能选择设置
 * @param       GPIOx: GPIOA~GPIOI, GPIO指针
 * @param       PINx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       AFx:0~15, 代表AF0~AF15.
 *              AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
 *   @arg       AF0:MCO/SWD/SWCLK/RTC;      AF1:TIM1/TIM2;                  AF2:TIM3~5;                     AF3:TIM8~11
 *   @arg       AF4:I2C1~I2C4;              AF5:SPI1~SPI6;                  AF6:SPI3/SAI1;                  AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
 *   @arg       AF8:USART4~8/SPDIFRX/SAI2;  AF9;CAN1~2/TIM12~14/LCD/QSPI;   AF10:USB_OTG/USB_HS/SAI2/QSPI;  AF11:ETH;
 *   @arg       AF12:FMC/SDIO/OTG/HS;       AF13:DCIM;                      AF14:LCD;                       AF15:EVENTOUT;
 * @retval      无
 */
void bsp_gpio_af_set(GPIO_TypeDef *GPIOx, uint16_t PINx, uint8_t AFx)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;;

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = PINx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            GPIOx->AFR[pinpos >> 3] &= ~(0X0F << ((pinpos & 0X07) * 4));
            GPIOx->AFR[pinpos >> 3] |= (uint32_t)AFx << ((pinpos & 0X07) * 4);
        }
    }
}


/**
 * @brief       设置GPIO某个引脚的输出状态
 * @param       p_gpiox: GPIOA~GPIOI, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, 引脚状态(仅最低位有效), 设置如下:
 *   @arg       0, 输出低电平
 *   @arg       1, 输出高电平
 * @retval      无
 */
void bsp_gpio_pin_set(GPIO_TypeDef *GPIOx, uint16_t PINx, uint8_t status)
{
    if (status & 0X01)
    {
        GPIOx->BSRR |= PINx;  /* 设置GPIOx的pinx为1 */
    }
    else
    {
        GPIOx->BSRR |= (uint32_t)PINx << 16;  /* 设置GPIOx的pinx为0 */
    }
}


/**
 * @brief       读取GPIO某个引脚的状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 一次只能读取一个GPIO！
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @retval      返回引脚状态, 0, 低电平; 1, 高电平
 */
uint8_t bsp_gpio_pin_get(GPIO_TypeDef *GPIOx, uint16_t PINx)
{
    if (GPIOx->IDR & PINx)
    {
        return 1;   /* pinx的状态为1 */
    }
    else
    {
        return 0;   /* pinx的状态为0 */
    }
}

/*
    专门用来兼容SDRAM的GPIO操作
*/
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
} 

