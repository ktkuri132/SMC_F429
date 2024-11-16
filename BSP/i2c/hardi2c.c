#include <stm32f4xx.h>

#include <gpio.h>

#include <hardi2c.h>

// 硬件I2C初始化
void Hard_I2C_Init()
{
    // 使能GPIOB时钟
    RCC->AHB1ENR |= 1 << 1;
    // 使能I2C1时钟
    RCC->APB1ENR |= 1 << 21;
    
    // 配置I2C1引脚
    bsp_gpio_init(GPIOB, SCL, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, SDA, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_OD, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    
    // 配置I2C1 AF
    bsp_gpio_af_set(GPIOB, SCL, 4);
    bsp_gpio_af_set(GPIOB, SDA, 4);
    
    // 配置I2C1
    I2C1->CR1 &= ~I2C_CR1_PE;   // 关闭I2C1
    I2C1->CR2 = 45;             // APB1时钟45M
    I2C1->CCR = 38;            // 速率为400K
    I2C1->TRISE = 46;           // 最大的I2C时钟周期为1000ns
    I2C1->CR1 |= I2C_CR1_PE;    // 使能I2C1
}

// 硬件IIC读函数
uint8_t Hard_I2C_Read(uint8_t device_address, uint8_t register_address)
{
    uint8_t data;

    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C1->DR = device_address << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = register_address;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成重复起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和读命令
    I2C1->DR = (device_address << 1) | 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    I2C1->CR1 &= ~I2C_CR1_ACK;  // 禁用ACK
    (void)I2C1->SR2;  // 清除ADDR标志

    // 读取数据
    while (!(I2C1->SR1 & I2C_SR1_RXNE));  // 等待数据接收完毕
    data = I2C1->DR;

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;

    return data;
}

// 硬件IIC写函数
uint8_t Hard_I2C_Write(uint8_t device_address, uint8_t register_address, uint8_t data)
{
    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C1->DR = device_address << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = register_address;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 发送数据
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;
    return 0;
}

// 硬件IIC连续写函数
uint8_t Hard_I2C_Write_Multiple(uint8_t device_address, uint8_t register_address, uint8_t* data, uint8_t length)
{
    
    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C1->DR = device_address << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = register_address;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 发送数据
    for (uint16_t i = 0; i < length; i++)
    {
        I2C1->DR = data[i];
        while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空
    }

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;

    return 0;
}

uint8_t __Hard_I2C_Write_Multiple(uint8_t device_address, uint8_t register_address, uint8_t length,uint8_t* data)
{
    
   Hard_I2C_Write_Multiple(device_address,register_address,data,length);
   return 0;
}

// 硬件IIC连续读函数
uint8_t Hard_I2C_Read_Multiple(uint8_t device_address, uint8_t register_address, uint8_t* data, uint16_t length)
{
    
    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕
    
    // 发送从设备地址和写命令
    I2C1->DR = device_address << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志
    
    // 发送寄存器地址
    I2C1->DR = register_address;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成重复起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和读命令
    I2C1->DR = (device_address << 1) | 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    uint16_t i = 0;
    for (i = 0; i < length; i++)
    {
        while (!(I2C1->SR1 & I2C_SR1_RXNE));  // 等待数据接收完毕
        data[i] = I2C1->DR;
        if (i == length - 1)
        {
            I2C1->CR1 &= ~I2C_CR1_ACK;  // 禁用ACK
            // 倒数第1个字节时，生成停止条件
            I2C1->CR1 |= I2C_CR1_STOP;  // 生成停止条件
        }
    }

    return 0;
}



uint8_t __Hard_I2C_Read_Multiple(uint8_t device_address, uint8_t register_address,  uint16_t length,uint8_t* data)
{
    return Hard_I2C_Read_Multiple(device_address,register_address,data,length);
}