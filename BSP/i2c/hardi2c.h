#ifndef On_Chip_IIC
#define On_Chip_IIC

#ifndef  __HARDI2C_
#define  __HARDI2C_

#include <stm32f4xx.h>

// 定义部分硬件I2C端口
//#define I2C1_B6C7D
#define I2C_Port I2C1
//#define I2C2_B10C11D

#ifdef  I2C_Port
#define HARD_IIC_PORT GPIOB
#define HARD_IIC_SCL_PIN GPIO_Pin_8
#define HARD_IIC_SDA_PIN GPIO_Pin_9
#define HARD_IIC_SCL_PORT 8
#define HARD_IIC_SDA_PORT 9
#endif

void Hard_IIC_Init();
uint8_t Hard_IIC_Send_Byte(uint8_t Address,uint8_t Register,uint8_t Byte);
uint8_t Hard_IIC_Wirter_Data(uint8_t Address,uint8_t Register,uint8_t *Data,uint8_t Count);
uint8_t Hard_IIC_Receive_Byte(uint8_t Address,uint8_t Register);
uint8_t Hard_IIC_Read_Data(uint8_t Address,uint8_t Register,uint8_t *Data,uint8_t Count);




#endif
#endif