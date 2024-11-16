#ifndef  __HARDI2C_
#define  __HARDI2C_

#include <stm32f4xx.h>

// 定义部分硬件I2C端口
//#define I2C1_B6C7D
#define I2C1_B9C8D
//#define I2C2_B10C11D

#ifdef I2C1_B9C8D
#define SCL 1<<8
#define SDA 1<<9
#endif


void Hard_I2C_Init();
uint8_t Hard_I2C_Read(uint8_t device_address, uint8_t register_address);
uint8_t Hard_I2C_Write(uint8_t device_address, uint8_t register_address, uint8_t data);

uint8_t Hard_I2C_Write_Multiple(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length);
uint8_t __Hard_I2C_Write_Multiple(uint8_t device_address, uint8_t register_address, uint8_t length,uint8_t* data);

uint8_t Hard_I2C_Read_Multiple(uint8_t device_address, uint8_t register_address, uint8_t* data, uint16_t length);
uint8_t __Hard_I2C_Read_Multiple(uint8_t device_address, uint8_t register_address,  uint16_t length,uint8_t* data);





#endif