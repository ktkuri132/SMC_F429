#ifndef On_Chip_IIC
#define On_Chip_IIC

#ifndef __SOFTI2C_
#define __SOFTI2C_
#include "softi2c.h"
#include "stm32f4xx.h"
#include "gpio/gpio.h"
#define SOFT_IIC_PORT       GPIOB
#define SOFT_IIC_SCL_PORT   8
#define SOFT_IIC_SDA_PORT   9

#ifdef __STM32F4xx_GPIO_H
#define SCL_GPIO            GPIO_Pin_8
#define SDA_GPIO            GPIO_Pin_9
#endif

//IO方向设置
#define Soft_SDA_IN()  {SOFT_IIC_PORT->MODER&=~(3<<(SOFT_IIC_SDA_PORT*2));SOFT_IIC_PORT->MODER|=0<<SOFT_IIC_SDA_PORT*2;}	//PB9输入模式
#define Soft_SDA_OUT() {SOFT_IIC_PORT->MODER&=~(3<<(SOFT_IIC_SDA_PORT*2));SOFT_IIC_PORT->MODER|=1<<SOFT_IIC_SDA_PORT*2;} //PB9输出模式
//IO操作函数	 
#define Soft_IIC_SCL    PBout(SOFT_IIC_SCL_PORT) //SCL
#define Soft_IIC_SDA    PBout(SOFT_IIC_SDA_PORT) //SDA	 
#define Soft_READ_SDA   PBin(SOFT_IIC_SDA_PORT)  //输入SDA 

//IIC所有操作函数
void Soft_IIC_Init(void);                //初始化IIC的IO口				 
void Soft_IIC_Start(void);				//发送IIC开始信号
void Soft_IIC_Stop(void);	  			//发送IIC停止信号
void Soft_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 Soft_IIC_Receive_Byte(unsigned char ack);//IIC读取一个字节
u8 Soft_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void Soft_IIC_Ack(void);					//IIC发送ACK信号
void Soft_IIC_NAck(void);				//IIC不发送ACK信号

u8 Soft_IIC_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);//IIC连续写
u8 Soft_IIC_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); //IIC连续读 
u8 Soft_IIC_Write_Byte(u8 addr,u8 reg,u8 data);				//IIC写一个字节
u8 Soft_IIC_Read_Byte(u8 addr,u8 reg);						//IIC读一个字节

#endif
#endif
