#include "stm32f4xx.h"
#include <gpio.h>
#include <softi2c.h>

// 初始化软件I2C
void Soft_I2C_Init(void) 
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 

    bsp_gpio_init(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN, SYS_GPIO_MODE_OUT, 
                    SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_NONE);
    bsp_gpio_init(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN, SYS_GPIO_MODE_OUT,
                    SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_NONE);
}


void Soft_I2C_Start(void)
{
	Soft_W_SDA(1);		//释放SDA，确保SDA为高电平
	Soft_W_SCL(1);		//释放SCL，确保SCL为高电平
	Soft_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
	Soft_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}


void Soft_I2C_Stop(void)
{
	Soft_W_SDA(0);		//拉低SDA，确保SDA为低电平
	Soft_W_SCL(1);		//释放SCL，使SCL呈现高电平
	Soft_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}


void Soft_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	/*循环8次，主机依次发送数据的每一位*/
	for (i = 0; i < 8; i++)
	{
		/*使用掩码的方式取出Byte的指定一位数据并写入到SDA线*/
		/*两个!的作用是，让所有非零的值变为1*/
		Soft_W_SDA(!!(Byte & (0x80 >> i)));
		Soft_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
		Soft_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
	}
	
	Soft_W_SCL(1);		//额外的一个时钟，不处理应答信号
	Soft_W_SCL(0);
}


void Soft_I2C_WriteByte(uint8_t Adder,uint8_t Register,uint8_t Command)
{
	Soft_I2C_Start();				//I2C起始
	Soft_I2C_SendByte(Adder);		//发送Soft的I2C从机地址
	Soft_I2C_SendByte(Register);		//控制字节，给0x00，表示即将写命令
	Soft_I2C_SendByte(Command);		//写入指定的命令
	Soft_I2C_Stop();				//I2C终止
}


void Soft_I2C_WriteData(uint8_t Adder,uint8_t Register,uint8_t *Data, uint8_t Count)
{
	uint8_t i;
	
	Soft_I2C_Start();				//I2C起始
	Soft_I2C_SendByte(Adder);		//发送Soft的I2C从机地址
	Soft_I2C_SendByte(Register);		//控制字节，给0x40，表示即将写数据
	/*循环Count次，进行连续的数据写入*/
	for (i = 0; i < Count; i ++)
	{
		Soft_I2C_SendByte(Data[i]);	//依次发送Data的每一个数据
	}
	Soft_I2C_Stop();				//I2C终止
}