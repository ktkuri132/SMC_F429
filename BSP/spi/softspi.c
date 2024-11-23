#include <softspi.h>

#ifdef __STM32F4xx_GPIO_H

#define Soft_W_SCK(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(x))
#define Soft_W_MOSI(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(x))
#define Soft_W_CS1(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(x))
#define Soft_W_CS2(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)(x))

#else 

#define Soft_W_SCK(x)		bsp_gpio_pin_set(SOFT_SPI_SCK_PORT, SOFT_SPI_SCK_PIN, x)
#define Soft_W_MOSI(x)		bsp_gpio_pin_set(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN, x)
#define Soft_R_MISO()		bsp_gpio_pin_get(SOFT_SPI_MISO_PORT, SOFT_SPI_MISO_PIN)
#define Soft_W_CS1(x)		bsp_gpio_pin_set(SOFT_SPI_CS1_PORT, SOFT_SPI_CS1_PIN, x)
#define Soft_W_CS2(x)		bsp_gpio_pin_set(SOFT_SPI_CS2_PORT, SOFT_SPI_CS2_PIN, x)
#define Soft_W_CS3(x)		bsp_gpio_pin_set(SOFT_SPI_CS3_PORT, SOFT_SPI_CS3_PIN, x)
#endif

/*引脚初始化*/
void Soft_SPI_Init(void)
{
    #ifdef __STM32F4xx_GPIO_H
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	#else
    bsp_gpio_init(GPIOB, SOFT_SPI_SCK_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, SOFT_SPI_MOSI_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, SOFT_SPI_MISO_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOA, SOFT_SPI_CS1_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOA, SOFT_SPI_CS2_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOA, SOFT_SPI_CS3_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    #endif
	Soft_W_SCK(0);
	Soft_W_CS(1);
}

/**
  * @brief  SPI发送一个字节
  * @param  Byte 要发送的一个字节
  * @param  CS 片选
  * @retval 无
  */
void Soft_SPI_SendByte(uint8_t CS,uint8_t Byte)
{
    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(0);
        }
        break;
        case 2:
        {
            Soft_W_CS2(0);
        }
        break;
        case 3:
        {
            Soft_W_CS3(0);
        }
        default:
            break;
    }

	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		Soft_W_MOSI(Byte & (0x80 >> i));
		Soft_W_SCK(1);
		Soft_W_SCK(0);
	}

    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(1);
        }
        break;
        case 2:
        {
            Soft_W_CS2(1);
        }
        break;
        case 3:
        {
            Soft_W_CS3(1);
        }
        default:
            break;
    }
}


void Soft_SPI_SendData(uint8_t CS,uint8_t *Data,uint8_t Count)
{
    uint8_t i;
    for(i=0;i<Count;i++)
    {
        Soft_SPI_SendByte(Data[i],CS);
    }
}

/// @brief SPI接收一个字节
/// @param CS 片选
/// @return 若不启用缓冲数据，则正常返回接收数据
uint8_t Soft_SPI_ReceiveByte(uint8_t CS)
{
    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(0);
        }
        break;
        case 2:
        {
            Soft_W_CS2(0);
        }
        break;
        case 3:
        {
            Soft_W_CS3(0);
        }
        default:
            break;
    }

    uint8_t ByteReceive = 0x00;
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(Soft_R_MISO()==1)
        {
            ByteReceive |= (0x80 >> i);
        }
        Soft_W_SCK(1);
		Soft_W_SCK(0);
    }

    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(1);
        }
        break;
        case 2:
        {
            Soft_W_CS2(1);
        }
        break;
        case 3:
        {
            Soft_W_CS3(1);
        }
        default:
            break;
    }

    return ByteReceive;
}

void Soft_SPI_ReceiveData(uint8_t CS,uint8_t *Data,uint8_t Count)
{
    uint8_t i;
    for(i=0;i<Count;i++)
    {
        *Data = Soft_SPI_ReceiveByte(CS);
    }
}


/**
  * @brief  SPI交换数据，同时收发
  * @param  SData 要写入的字节
  * @param  CS 片选
  * @retval 无
  */
void Soft_SwapData(uint8_t CS,uint8_t SData)
{
    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(0);
        }
        break;
        case 2:
        {
            Soft_W_CS2(0);
        }
        break;
        case 3:
        {
            Soft_W_CS3(0);
        }
        default:
            break;
    }

    uint8_t i,ByteReceive = 0x00;
    for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
	{
		Soft_W_MOSI(SData & (0x80 >> i));		//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
		Soft_W_SCK(1);								//拉高SCK，上升沿移出数据
		if (Soft_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	//读取MISO数据，并存储到Byte变量
																//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
		Soft_W_SCK(0);								//拉低SCK，下降沿移入数据
	}

    switch (CS)
    {
        case 1:
        {
            Soft_W_CS1(1);
        }
        break;
        case 2:
        {
            Soft_W_CS2(1);
        }
        break;
        case 3:
        {
            Soft_W_CS3(1);
        }
        default:
            break;
    }
}


