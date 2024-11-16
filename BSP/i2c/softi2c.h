#ifndef _SOFTI2C_
#define _SOFTI2C_

#include <Project.h>

// 定义IIC端口
#define SOFT_I2C_SCL_PIN SYS_GPIO_PIN8
#define SOFT_I2C_SDA_PIN SYS_GPIO_PIN9
#define SOFT_I2C_GPIO_PORT GPIOB

// 函数声明
void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
void Soft_I2C_WriteBit(uint8_t bit);
uint8_t Soft_I2C_ReadBit(void);
void Soft_I2C_WriteByte(uint8_t byte);
uint8_t Soft_I2C_ReadByte(uint8_t ack);
uint8_t Soft_I2C_WaitAck(void);
void Soft_I2C_Ack(void);
void Soft_I2C_NAck(void);

uint8_t Soft_I2C_WriteData(uint8_t device_address, uint8_t register_address, uint8_t data);
uint8_t Soft_I2C_WriteFrame(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length);
uint8_t Soft_I2C_ReadFrame(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length);


// 延迟函数接口
#define delay_us(x) bsp_systick_delay_us(x)


#endif // !_SOFTI2C_