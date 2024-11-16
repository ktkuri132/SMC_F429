#include "stm32f4xx.h"
#include <gpio.h>
#include <softi2c.h>

// 初始化软件I2C
void Soft_I2C_Init(void) 
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 

    bsp_gpio_init(GPIOB, SOFT_I2C_SCL_PIN, SYS_GPIO_MODE_OUT, 
                    SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_NONE);
    bsp_gpio_init(GPIOB, SOFT_I2C_SDA_PIN, SYS_GPIO_MODE_OUT,
                    SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_NONE);
}

// 产生I2C起始信号
void Soft_I2C_Start(void) 
{
    GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    GPIOB->BSRR = (SOFT_I2C_SDA_PIN << 16); // SDA low
    delay_us(1);
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
}

// 产生I2C停止信号
void Soft_I2C_Stop(void) 
{
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    GPIOB->BSRR = (SOFT_I2C_SDA_PIN << 16); // SDA low
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    delay_us(1);
}

// 写入一个bit
void Soft_I2C_WriteBit(uint8_t bit) 
{
    if (bit) 
    {
        GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    } else 
    {
        GPIOB->BSRR = (SOFT_I2C_SDA_PIN << 16); // SDA low
    }
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    delay_us(1);
}

// 读取一个bit
uint8_t Soft_I2C_ReadBit(void) 
{
    uint8_t bit;
    GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    bit = (GPIOB->IDR & SOFT_I2C_SDA_PIN) ? 1 : 0;
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    delay_us(1);
    return bit;
}

// 写入一个字节
void Soft_I2C_WriteByte(uint8_t byte) 
{
    for (uint8_t i = 0; i < 8; i++) 
    {
        Soft_I2C_WriteBit(byte & 0x80);
        byte <<= 1;
    }
    Soft_I2C_WaitAck();
}

// 读取一个字节
uint8_t Soft_I2C_ReadByte(uint8_t ack) 
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) 
    {
        byte <<= 1;
        byte |= Soft_I2C_ReadBit();
    }
    if (ack) 
    {
        Soft_I2C_Ack();
    } else 
    {
        Soft_I2C_NAck();
    }
    return byte;
}

// 等待应答信号
uint8_t Soft_I2C_WaitAck(void) 
{
    uint8_t ack;
    GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    ack = (GPIOB->IDR & SOFT_I2C_SDA_PIN) ? 1 : 0;
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    delay_us(1);
    return ack;
}

// 产生一个应答
void Soft_I2C_Ack(void) 
{
    GPIOB->BSRR = (SOFT_I2C_SDA_PIN << 16); // SDA low
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    delay_us(1);
}

// 产生一个非应答
void Soft_I2C_NAck(void) 
{
    GPIOB->BSRR = SOFT_I2C_SDA_PIN; // SDA high
    delay_us(1);
    GPIOB->BSRR = SOFT_I2C_SCL_PIN; // SCL high
    delay_us(1);
    GPIOB->BSRR = (SOFT_I2C_SCL_PIN << 16); // SCL low
    delay_us(1);
}


// 写一帧数据:数据长度为1
uint8_t Soft_I2C_WriteData(uint8_t device_address, uint8_t register_address, uint8_t data) 
{
    Soft_I2C_Start();
    Soft_I2C_WriteByte(device_address);
    Soft_I2C_WriteByte(register_address);
    Soft_I2C_WriteByte(data);
    Soft_I2C_Stop();
    return 0;
}

// 写一帧数据：数据长度大于1
uint8_t Soft_I2C_WriteFrame(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length) 
{
    Soft_I2C_Start();
    Soft_I2C_WriteByte(device_address);
    Soft_I2C_WriteByte(register_address);
    for (uint8_t i = 0; i < length; i++) 
    {
        Soft_I2C_WriteByte(data[i]);
    }
    Soft_I2C_Stop();
    return 0;
}

// 读一帧数据
uint8_t Soft_I2C_ReadFrame(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length) 
{
    Soft_I2C_Start();
    Soft_I2C_WriteByte(device_address);
    Soft_I2C_WriteByte(register_address);
    Soft_I2C_Start();
    Soft_I2C_WriteByte(device_address | 0x01);
    for (uint8_t i = 0; i < length; i++) 
    {
        data[i] = Soft_I2C_ReadByte(i == length - 1);
    }
    Soft_I2C_Stop();
    return 0;
}