#include "softi2c.h"     // 此处定义片上软件IIC


void delay_us(uint32_t nus)
{
	SysTick->LOAD = nus * 1; 
    SysTick->VAL = 0x00; 
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
    {
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 
    SysTick->VAL = 0X00;	
}

//初始化IIC
void Soft_IIC_Init(void)
{			

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	#ifdef __STM32F4xx_GPIO_H
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO | SDA_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	#else
	bsp_gpio_init(GPIOB, 1<<8, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, 1<<9, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_OD, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
	#endif
	Soft_IIC_SCL=1;
	Soft_IIC_SDA=1;
}

//产生IIC起始信号
void Soft_IIC_Start(void)
{
	Soft_SDA_OUT();     //sda线输出
	Soft_IIC_SDA=1;	  	  
	Soft_IIC_SCL=1;
	// delay_us(4);
 	Soft_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	Soft_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void Soft_IIC_Stop(void)
{
	Soft_SDA_OUT();//sda线输出
	Soft_IIC_SCL=0;
	Soft_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	// delay_us(4);
	Soft_IIC_SCL=1; 
	Soft_IIC_SDA=1;//发送I2C总线结束信号
	// delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 Soft_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	Soft_SDA_IN();      //SDA设置为输入  
	Soft_IIC_SDA=1;delay_us(1);	   
	Soft_IIC_SCL=1;delay_us(1);	 
	while(Soft_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Soft_IIC_Stop();
			return 1;
		}
	}
	Soft_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void Soft_IIC_Ack(void)
{
	Soft_IIC_SCL=0;
	Soft_SDA_OUT();
	Soft_IIC_SDA=0;
	delay_us(1);
	Soft_IIC_SCL=1;
	// delay_us(2);
	Soft_IIC_SCL=0;
}
//不产生ACK应答		    
void Soft_IIC_NAck(void)
{
	Soft_IIC_SCL=0;
	Soft_SDA_OUT();
	Soft_IIC_SDA=1;
	delay_us(1);
	Soft_IIC_SCL=1;
	// delay_us(2);
	Soft_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void Soft_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	Soft_SDA_OUT(); 	    
    Soft_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        Soft_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(1);   
		Soft_IIC_SCL=1;
		delay_us(1); 
		Soft_IIC_SCL=0;	
		// delay_us(1);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 Soft_IIC_Receive_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	Soft_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        Soft_IIC_SCL=0; 
        delay_us(1);
		Soft_IIC_SCL=1;
        receive<<=1;
        if(Soft_READ_SDA)receive++;   
		// delay_us(1); 
    }					 
    if (!ack)
        Soft_IIC_NAck();//发送nACK
    else
        Soft_IIC_Ack(); //发送ACK   
    return receive;
}

u8 Soft_IIC_Write_Byte(u8 addr,u8 reg,u8 data) 				 
{ 
    Soft_IIC_Start(); 
	Soft_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	if(Soft_IIC_Wait_Ack())	//等待应答
	{
		Soft_IIC_Stop();		 
		return 1;		
	}
    Soft_IIC_Send_Byte(reg);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答 
	Soft_IIC_Send_Byte(data);//发送数据
	if(Soft_IIC_Wait_Ack())	//等待ACK
	{
		Soft_IIC_Stop();	 
		return 1;		 
	}		 
    Soft_IIC_Stop();	 
	return 0;
}

u8 Soft_IIC_Read_Byte(u8 addr,u8 reg)
{
	u8 res;
    Soft_IIC_Start(); 
	Soft_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	Soft_IIC_Wait_Ack();		//等待应答 
    Soft_IIC_Send_Byte(reg);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答
    Soft_IIC_Start();
	Soft_IIC_Send_Byte(addr|1);//发送器件地址+读命令	
    Soft_IIC_Wait_Ack();		//等待应答 
	res=Soft_IIC_Receive_Byte(0);//读取数据,发送nACK 
    Soft_IIC_Stop();			//产生一个停止条件 
	return res;		
}

u8 Soft_IIC_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    Soft_IIC_Start(); 
	Soft_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	if(Soft_IIC_Wait_Ack())	//等待应答
	{
		Soft_IIC_Stop();		 
		return 1;		
	}
    Soft_IIC_Send_Byte(reg);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		Soft_IIC_Send_Byte(buf[i]);	//发送数据
		if(Soft_IIC_Wait_Ack())		//等待ACK
		{
			Soft_IIC_Stop();	 
			return 1;		 
		}		
	}    
    Soft_IIC_Stop();	 
	return 0;	
} 

u8 Soft_IIC_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	Soft_IIC_Start(); 
	Soft_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	if(Soft_IIC_Wait_Ack())	//等待应答
	{;
		Soft_IIC_Stop();
		return 1;
	}
    Soft_IIC_Send_Byte(reg);	//写寄存器地址
    Soft_IIC_Wait_Ack();		//等待应答
    Soft_IIC_Start();
	Soft_IIC_Send_Byte(addr|1);//发送器件地址+读命令	
    Soft_IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=Soft_IIC_Receive_Byte(0);//读数据,发送nACK 
		else *buf=Soft_IIC_Receive_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    Soft_IIC_Stop();	//产生一个停止条件 
	return 0;	
}



