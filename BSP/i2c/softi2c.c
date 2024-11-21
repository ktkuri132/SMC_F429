#include <softi2c.h>


//Soft IIC 延时函数
void Soft_IIC_Delay(void)
{
	bsp_systick_delay_us(2);
}

//初始化IIC
void Soft_IIC_Init(void)
{			
  				      
}
//产生IIC起始信号
void Soft_IIC_Start(void)
{
	Soft_SDA_OUT();     //sda线输出
	Soft_IIC_SDA=1;	  	  
	Soft_IIC_SCL=1;
	Soft_IIC_Delay();
 	Soft_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Soft_IIC_Delay();
	Soft_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void Soft_IIC_Stop(void)
{
	Soft_SDA_OUT();//sda线输出
	Soft_IIC_SCL=0;
	Soft_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Soft_IIC_Delay();
	Soft_IIC_SCL=1;  
	Soft_IIC_SDA=1;//发送I2C总线结束信号
	Soft_IIC_Delay();							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 Soft_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	Soft_SDA_IN();      //SDA设置为输入  
	Soft_IIC_SDA=1;Soft_IIC_Delay();	   
	Soft_IIC_SCL=1;Soft_IIC_Delay();	 
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
	Soft_IIC_Delay();
	Soft_IIC_SCL=1;
	Soft_IIC_Delay();
	Soft_IIC_SCL=0;
}
//不产生ACK应答		    
void Soft_IIC_NAck(void)
{
	Soft_IIC_SCL=0;
	Soft_SDA_OUT();
	Soft_IIC_SDA=1;
	Soft_IIC_Delay();
	Soft_IIC_SCL=1;
	Soft_IIC_Delay();
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
		Soft_IIC_SCL=1;
		Soft_IIC_Delay(); 
		Soft_IIC_SCL=0;	
		Soft_IIC_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 Soft_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	Soft_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        Soft_IIC_SCL=0; 
        Soft_IIC_Delay();
		Soft_IIC_SCL=1;
        receive<<=1;
        if(Soft_READ_SDA)receive++;   
		Soft_IIC_Delay(); 
    }					 
    if (!ack)
        Soft_IIC_NAck();//发送nACK
    else
        Soft_IIC_Ack(); //发送ACK   
    return receive;
}
