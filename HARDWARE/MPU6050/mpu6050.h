#ifndef __MPU6050_
#define __MPU6050_

typedef unsigned char u8;
typedef unsigned short u16;


/* 外部驱动头文件  */
// #include <hardi2c.h>
#include <softi2c.h>
#include <sys.h>

#define MPU_SCL 8
#define MPU_SDA 9

#ifdef On_Chip_IIC
#undef Peripheral_IIC

#ifdef __HARDI2C_
#define MPU6050_ADDR 0x68

#define MPU_Write_Byte(reg,data) Hard_I2C_Write(MPU6050_ADDR,reg,data)
#define MPU_Write_Len(addr,reg,len,buf) Hard_I2C_Write_Multiple(addr,reg,buf,len)
#define MPU_Read_Len(addr,reg,len,buf) Hard_I2C_Read_Multiple(addr,reg,buf,len)
#define MPU_IIC_Init() Hard_I2C_Init()

#define MPU_Read_Byte(reg) Hard_I2C_Read(MPU6050_ADDR,reg)

#elif defined __SOFTI2C_
#define MPU6050_ADDR 0x68
#define MPU_Write_Byte(reg,data) Soft_IIC_WriteByte(MPU6050_ADDR,reg,data)
#define MPU_Write_Len(addr,reg,len,buf) Soft_IIC_WriteData(addr,reg,buf,len)
#define MPU_Read_Len(addr,reg,len,buf) Soft_IIC_ReadData(addr,reg,buf,len)
#define MPU_IIC_Init() Soft_IIC_Init()

#endif

#endif

#endif
