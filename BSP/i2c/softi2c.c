#include <softi2c.h>

//* ------------------------------------------底层----------------------------------------------*/

void Soft_W_SCL(uint8_t BitVal)  
{
	#ifndef __STM32F4xx_GPIO_H
	bsp_gpio_pin_set(SOFT_I2C_GPIO_PORT,SOFT_I2C_SCL_PIN,BitVal);
	#else
	GPIO_WriteBit(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PORT, (BitAction)BitValue);
	#endif
}


void Soft_W_SDA(uint8_t BitVal)
{
	
	#ifndef __STM32F4xx_GPIO_H
	bsp_gpio_pin_set(SOFT_I2C_GPIO_PORT,SOFT_I2C_SDA_PIN,BitVal);
	#else
	GPIO_WriteBit(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PORT, (BitAction)BitValue);
	#endif
	
}

/*
	软件IIC的GPIO初始化
*/
void Soft_IIC_Init(void)
{
	uint32_t i, j;
	
	/*在初始化前，加入适量延时，待Soft供电稳定*/
	for (i = 0; i < 1000; i ++)
	{
		for (j = 0; j < 1000; j ++);
	}
	#ifndef __STM32F4xx_GPIO_H
	/*将SCL和SDA引脚初始化为开漏模式*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	bsp_gpio_init(SOFT_I2C_GPIO_PORT,SOFT_I2C_SDA_PIN,\
				SYS_GPIO_MODE_OUT,SYS_GPIO_OTYPE_OD,SYS_GPIO_SPEED_HIGH,SYS_GPIO_PUPD_PU);
	bsp_gpio_init(SOFT_I2C_GPIO_PORT,SOFT_I2C_SCL_PIN,\
				SYS_GPIO_MODE_OUT,SYS_GPIO_OTYPE_PP,SYS_GPIO_SPEED_HIGH,SYS_GPIO_PUPD_PU);
	#else
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	/*释放SCL和SDA*/
	Soft_W_SCL(1);
	Soft_W_SDA(1);
}

void Soft_IIC_Delay()
{
	bsp_systick_delay_us(2);
}


uint8_t Soft_R_SDA()
{
	uint8_t bitstatus = 0x00;

	if ((SOFT_I2C_GPIO_PORT->IDR & SOFT_I2C_SDA_PIN) != (uint32_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	return bitstatus;
}

//*-------------------------------------------协议层-------------------------------------------------*/

/*
	起始
*/
void Soft_IIC_Start(void)
{
	Soft_W_SDA(1);		//释放SDA，确保SDA为高电平
	Soft_W_SCL(1);		//释放SCL，确保SCL为高电平
	Soft_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
	Soft_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/*
	结束
*/
void Soft_IIC_Stop(void)
{
	Soft_W_SDA(0);		//拉低SDA，确保SDA为低电平
	Soft_W_SCL(1);		//释放SCL，使SCL呈现高电平
	Soft_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}


/*
	产生ACK应答
*/
void Soft_IIC_Ack(void)
{
	Soft_W_SCL(0);
	Soft_W_SDA(0);
	Soft_IIC_Delay();
	Soft_W_SCL(1);
	Soft_IIC_Delay();
	Soft_W_SCL(0);
}
/*
	不产生ACK应答(NACK)
*/		    
void Soft_IIC_NAck(void)
{
	Soft_W_SCL(0);
	Soft_W_SDA(1);
	Soft_IIC_Delay();
	Soft_W_SCL(1);
	Soft_IIC_Delay();
	Soft_W_SCL(0);
}	

/*
	等待IIC总线的应答
*/
uint8_t Soft_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	// Soft_SDA_IN();      //SDA设置为输入  
	Soft_W_SDA(1);//Soft_IIC_Delay();	   
	Soft_W_SCL(1);//Soft_IIC_Delay();	 
	while(Soft_R_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Soft_IIC_Stop();
			return 1;
		}
	}
	Soft_W_SCL(0);//时钟输出0 	   
	return 0;  
}

/*
	基本IIC发送一个字节
*/
void Soft_IIC_SendByte(uint8_t Byte)
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


/*
	基本IIC读取一个字节，ack=1时，发送ACK，ack=0，发送nACK
*/   
uint8_t Soft_IIC_ReceiveByte(unsigned char ack)
{
	unsigned char i,receive=0;
    for(i=0;i<8;i++ )
	{
        Soft_W_SCL(0); 
        Soft_IIC_Delay();
		Soft_W_SCL(1);
        receive<<=1;
        if(Soft_R_SDA())receive++;   
		Soft_IIC_Delay(); 
    }					 
    if (!ack)
        Soft_IIC_NAck();//发送nACK
    else
        Soft_IIC_Ack(); //发送ACK   
    return receive;
}


//*---------------------------------------------应用层-------------------------------------------------*/

/*
	向设备写一个字节
*/
void Soft_IIC_WriteByte(uint8_t Address,uint8_t Register,uint8_t Command)
{
	Soft_IIC_Start();				//IIC起始
	Soft_IIC_SendByte(Address);		//发送Soft的IIC从机地址
	Soft_IIC_SendByte(Register);		//控制字节，给0x00，表示即将写命令
	Soft_IIC_SendByte(Command);		//写入指定的命令
	Soft_IIC_Stop();				//IIC终止
}

/*
	向设备写指定长度字节
*/
uint16_t Soft_IIC_WriteData(uint8_t Address,uint8_t Register,uint8_t *Data, uint8_t Count)
{
	uint8_t i;
	
	Soft_IIC_Start();				//IIC起始
	Soft_IIC_SendByte(Address);		//发送Soft的IIC从机地址
	Soft_IIC_SendByte(Register);		//控制字节，给0x40，表示即将写数据
	/*循环Count次，进行连续的数据写入*/
	for (i = 0; i < Count; i ++)
	{
		Soft_IIC_SendByte(Data[i]);	//依次发送Data的每一个数据
	}
	Soft_IIC_Stop();				//IIC终止
	return 0;
}


/*
	向设备读取一个字节
*/
uint8_t Soft_IIC_ReadByte(uint8_t Address,uint8_t Register)
{
	uint8_t res;
    Soft_IIC_Start(); 
	Soft_IIC_SendByte((Address<<1)|0);//发送器件地址+写命令	
	Soft_IIC_Wait_Ack();		//等待应答 
    Soft_IIC_SendByte(Register);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答
    Soft_IIC_Start();
	Soft_IIC_SendByte((Address<<1)|1);//发送器件地址+读命令	
    Soft_IIC_Wait_Ack();		//等待应答 
	res=Soft_IIC_ReceiveByte(0);//读取数据,发送nACK 
    Soft_IIC_Stop();			//产生一个停止条件 
	return res;		
}


/*
	向设备读取指定长度字节
*/
uint8_t Soft_IIC_ReadData(uint8_t Address,uint8_t Register,uint8_t len,uint8_t *buf)
{ 
 	Soft_IIC_Start(); 
	Soft_IIC_SendByte((Address<<1)|0);//发送器件地址+写命令	
	if(Soft_IIC_Wait_Ack())	//等待应答
	{
		Soft_IIC_Stop();		 
		return 1;		
	}
    Soft_IIC_SendByte(Register);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答
    Soft_IIC_Start();
	Soft_IIC_SendByte((Address<<1)|1);//发送器件地址+读命令	
    Soft_IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=Soft_IIC_ReceiveByte(0);//读数据,发送nACK 
		else *buf=Soft_IIC_ReceiveByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    Soft_IIC_Stop();	//产生一个停止条件 
	return 0;	
}