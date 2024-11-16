#ifndef __MPU6050_
#define __MPU6050_

typedef unsigned char u8;
typedef unsigned short u16;


/* 外部驱动头文件  */
#include <hardi2c.h>
#include <systick.h>
#include <gpio.h>

#define MPU6050_ADDR 0x68

//#define MPU_IIC_PORT i2c1
#define MPU_SCL 8
#define MPU_SDA 9

#define MPU_Write_Byte(reg,data) Hard_I2C_Write(MPU6050_ADDR,reg,data)
#define MPU_Write_Len(addr,reg,len,buf) __Hard_I2C_Write_Multiple(addr,reg,len,buf)
#define MPU_Read_Len(addr,reg,len,buf) __Hard_I2C_Read_Multiple(addr,reg,len,buf)
#define MPU_IIC_Init() Hard_I2C_Init()

#define MPU_Read_Byte(reg) Hard_I2C_Read(MPU6050_ADDR,reg)


#endif
