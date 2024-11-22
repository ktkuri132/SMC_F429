#ifndef _SOFTI2C_
#define _SOFTI2C_

#include <Project.h>
#include <sys.h>	

// 定义IIC端口
#define SOFT_I2C_SCL_PIN SYS_GPIO_PIN8
#define SOFT_I2C_SDA_PIN SYS_GPIO_PIN9
// defind IIC port (if be used)
#define SOFT_I2C_SCL_PORT PIN8
#define SOFT_I2C_SDA_PORT PIN9
#define SOFT_I2C_GPIO_PORT GPIOB

//#define Soft_W_SDA(x) bsp_gpio_pin_set(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN, x)
//#define Soft_W_SCL(x) bsp_gpio_pin_set(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN, x)

// 函数声明
void Soft_IIC_Init(void);
void Soft_IIC_WriteByte(uint8_t Address,uint8_t Register,uint8_t Command);
void Soft_IIC_WriteData(uint8_t Address,uint8_t Register,uint8_t *Data, uint8_t Count);
uint8_t Soft_IIC_ReadByte(uint8_t Address,uint8_t Register);
uint8_t Soft_IIC_ReadData(uint8_t Address,uint8_t Register,uint8_t len,uint8_t *buf);


// 延迟函数接口
#define Soft_I2C_Delay(x) bsp_systick_delay_us(x)


#endif // !_SOFTI2C_