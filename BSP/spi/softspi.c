#include "softspi.h"
#include <sys.h>

#ifndef Soft_SPI_W_CS

void Soft_SPI_W_CS(uint8_t BitValue)
{
	#ifdef __STM32F4xx_GPIO_H
	GPIO_WriteBit(SOFT_SPI_CS_PORT, SOFT_SPI_CS_PIN, (BitAction)BitValue);
	#else
	bsp_gpio_pin_set(SOFT_SPI_CS_PORT,SOFT_SPI_CS_PIN,BitValue);
	#endif
}

void Soft_SPI_W_SCK(uint8_t BitValue)
{
	#ifdef __STM32F4xx_GPIO_H
	GPIO_WriteBit(SOFT_SPI_SCK_PORT, SOFT_SPI_SCK_PIN, (BitAction)BitValue);
	#else
	bsp_gpio_pin_set(SOFT_SPI_SCK_PORT,SOFT_SPI_SCK_PIN,BitValue);
	#endif
}

void Soft_SPI_W_MOSI(uint8_t BitValue)
{
	#ifdef __STM32F4xx_GPIO_H
	GPIO_WriteBit(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN, (BitAction)BitValue);
	#else
	bsp_gpio_pin_set(SOFT_SPI_MOSI_PORT,SOFT_SPI_MOSI_PIN,BitValue);
	#endif
}

uint8_t Soft_SPI_R_MISO(void)
{
	#ifdef __STM32F4xx_GPIO_H
	return GPIO_ReadInputDataBit(SOFT_SPI_MISO_PORT, SOFT_SPI_MISO_PIN);
	#else
	return bsp_gpio_pin_get(SOFT_SPI_MISO_PORT,SOFT_SPI_MISO_PIN);
	#endif
}

#endif

/*引脚初始化*/
void Soft_SPI_Init(void)
{
    #ifdef __STM32F4xx_GPIO_H
    RCC_AHB1PeriphClockCmd(GetGPIOPeriphClock(SOFT_SPI_CS_PORT), ENABLE);
    RCC_AHB1PeriphClockCmd(GetGPIOPeriphClock(SOFT_SPI_MOSI_PORT), ENABLE);
    RCC_AHB1PeriphClockCmd(GetGPIOPeriphClock(SOFT_SPI_MISO_PORT), ENABLE);
    RCC_AHB1PeriphClockCmd(GetGPIOPeriphClock(SOFT_SPI_SCK_PORT), ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_CS_PIN;
    GPIO_Init(SOFT_SPI_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SOFT_SPI_SCK_PIN;
    GPIO_Init(SOFT_SPI_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_MOSI_PIN;
    GPIO_Init(SOFT_SPI_MOSI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_MISO_PIN;
    GPIO_Init(SOFT_SPI_MISO_PORT, &GPIO_InitStructure);

	#else
    // 初始化通用接口
    bsp_gpio_init(SOFT_SPI_SCK_PORT, SOFT_SPI_SCK_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(SOFT_SPI_MISO_PORT, SOFT_SPI_MISO_PIN, SYS_GPIO_MODE_IN, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(SOFT_SPI_CS_PORT, SOFT_SPI_CS_PIN, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    #endif
    Soft_SPI_W_CS(1);
	Soft_SPI_W_SCK(0);
}

/**
  * @brief  SPI发送一个字节
  * @param  Byte 要发送的一个字节
  * @param  CS 片选
  * @retval 无
  */
void Soft_SPI_SendByte(uint8_t CS,uint8_t Byte)
{
    Soft_SPI_W_CS(0);

	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		Soft_SPI_W_MOSI(!!Byte & (0x80 >> i));

		Soft_SPI_W_SCK(1);
		Soft_SPI_W_SCK(0);
	}

    Soft_SPI_W_CS(1);
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
    Soft_SPI_W_CS(0);

    uint8_t ByteReceive = 0x00;
    uint8_t i;
    for(i=0;i<8;i++)
    {
        /*
            兼容协议......
        */
        if(Soft_SPI_R_MISO()==1)
        {
            ByteReceive |= (0x80 >> i);
        }
        Soft_SPI_W_SCK(1);
		Soft_SPI_W_SCK(0);
    }

    Soft_SPI_W_CS(1);

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
  * @retval 无
  */
uint8_t Soft_SPI_SwapByte(uint8_t SData)
{
    uint8_t i,ByteReceive = 0x00;
    for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
	{
		Soft_SPI_W_MOSI(SData & (0x80 >> i));		//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
		Soft_SPI_W_SCK(1);								//拉高SCK，上升沿移出数据
		if (Soft_SPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	//读取MISO数据，并存储到Byte变量
																//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
		Soft_SPI_W_SCK(0);								//拉低SCK，下降沿移入数据
	}
    return ByteReceive;

}


