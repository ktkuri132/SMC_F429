#include <stm32f4xx.h>                  // Device header
#include <gpio/stm32f4xx_gpio.h>
#include "spiFlash.h"
/*引脚配置层*/

// /**
//   * 函    数：SPI写SS引脚电平
//   * 参    数：BitValue 协议层传入的当前需要写入SS的电平，范围0~1
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SS为低电平，当BitValue为1时，需要置SS为高电平
//   */
// void Soft_SPI_W_CS(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)BitValue);		//根据BitValue，设置SS引脚的电平
// }
//
// /**
//   * 函    数：SPI写SCK引脚电平
//   * 参    数：BitValue 协议层传入的当前需要写入SCK的电平，范围0~1
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCK为低电平，当BitValue为1时，需要置SCK为高电平
//   */
// void Soft_SPI_W_SCK(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)BitValue);		//根据BitValue，设置SCK引脚的电平
// }
//
// /**
//   * 函    数：SPI写MOSI引脚电平
//   * 参    数：BitValue 协议层传入的当前需要写入MOSI的电平，范围0~1
//   * 返 回 值：无
//   * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置MOSI为低电平，当BitValue为1时，需要置MOSI为高电平
//   */
// void Soft_SPI_W_MOSI(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOF, GPIO_Pin_9, (BitAction)BitValue);		//根据BitValue，设置MOSI引脚的电平，BitValue要实现非0即1的特性
// }
//
// /**
//   * 函    数：I2C读MISO引脚电平
//   * 参    数：无
//   * 返 回 值：协议层需要得到的当前MISO的电平，范围0~1
//   * 注意事项：此函数需要用户实现内容，当前MISO为低电平时，返回0，当前MISO为高电平时，返回1
//   */
// uint8_t Soft_SPI_R_MISO(void)
// {
// 	return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);			//读取MISO电平并返回
// }

/**
  * 函    数：SPI初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SS、SCK、MOSI和MISO引脚的初始化
  */
// void Soft_SPI_Init(void)
// {
// 	/*开启时钟*/
// 	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	//开启GPIOF的时钟
//
// 	/*GPIO初始化*/
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOF, &GPIO_InitStructure);					//将PF8引脚初始化为推挽输出
//
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOF, &GPIO_InitStructure);					//将PF9引脚初始化为上拉输入
//
// 	/*设置默认电平*/
// 	Soft_SPI_W_CS(1);											//SS默认高电平
// 	Soft_SPI_W_SCK(0);											//SCK默认低电平
// }

/*协议层*/

/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
// void Soft_SPI_Start(void)
// {
// 	Soft_SPI_W_CS(0);				//拉低SS，开始时序
// }
//
// /**
//   * 函    数：SPI终止
//   * 参    数：无
//   * 返 回 值：无
//   */
// void Soft_SPI_Stop(void)
// {
// 	Soft_SPI_W_CS(1);				//拉高SS，终止时序
// }

/**
  * 函    数：SPI交换传输一个字节，使用SPI模式0
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
// uint8_t Soft_SPI_SwapByte(uint8_t ByteSend)
// {
// 	uint8_t i, ByteReceive = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
//
// 	for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
// 	{
// 		/*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
// 		Soft_SPI_W_MOSI(!!(ByteSend & (0x80 >> i)));	//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
// 		Soft_SPI_W_SCK(1);								//拉高SCK，上升沿移出数据
// 		if (Soft_SPI_R_MISO()){ByteReceive |= (0x80 >> i);}	//读取MISO数据，并存储到Byte变量
// 															//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
// 		Soft_SPI_W_SCK(0);								//拉低SCK，下降沿移入数据
// 	}
//
// 	return ByteReceive;								//返回接收到的一个字节数据
// }

/**
  * 函    数：W25Q64初始化
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_Init(void)
{
	Soft_SPI_Init();					//先初始化基本的SPI引脚
}

/**
  * 函    数：W25Q64读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_JEDEC_ID);			//交换发送读取ID的指令
	*MID = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收MID，通过输出参数返回
	*DID = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收DID高8位
	*DID <<= 8;									//高8位移到高位
	*DID |= Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	Soft_SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64写使能
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WriteEnable(void)
{
	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_WRITE_ENABLE);		//交换发送写使能的指令
	Soft_SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64等待忙
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指令
	Timeout = 100000;							//给定超时计数时间
	while ((Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自减
		if (Timeout == 0)						//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;								//跳出等待，不等了
		}
	}
	Soft_SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64页编程
  * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray	用于写入数据的数组
  * 参    数：Count 要写入数据的数量，范围：0~256
  * 返 回 值：无
  * 注意事项：写入的地址范围不能跨页
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;

	W25Q64_WriteEnable();						//写使能

	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_PAGE_PROGRAM);		//交换发送页编程的指令
	Soft_SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	Soft_SPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		Soft_SPI_SwapByte(DataArray[i]);			//依次在起始地址后写入数据
	}
	Soft_SPI_Stop();								//SPI终止

	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64扇区擦除（4KB）
  * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * 返 回 值：无
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();						//写使能

	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	Soft_SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	Soft_SPI_SwapByte(Address);					//交换发送地址7~0位
	Soft_SPI_Stop();								//SPI终止

	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64读取数据
  * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * 参    数：Count 要读取数据的数量，范围：0~0x800000
  * 返 回 值：无
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	Soft_SPI_Start();								//SPI起始
	Soft_SPI_SwapByte(W25Q64_READ_DATA);			//交换发送读取数据的指令
	Soft_SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	Soft_SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	Soft_SPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		DataArray[i] = Soft_SPI_SwapByte(W25Q64_DUMMY_BYTE);	//依次在起始地址后读取数据
	}
	Soft_SPI_Stop();								//SPI终止
}