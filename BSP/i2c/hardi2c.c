#include "sys.h"

#ifdef  __HARDI2C_

void Hard_IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 配置I2C1引脚
    bsp_gpio_init(GPIOB, 1<<8, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_OD, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    bsp_gpio_init(GPIOB, 1<<9, SYS_GPIO_MODE_AF, SYS_GPIO_OTYPE_OD, SYS_GPIO_SPEED_FAST, SYS_GPIO_PUPD_PU);
    
    // 配置I2C1 AF
    bsp_gpio_af_set(GPIOB, 1<<8, 4);
    bsp_gpio_af_set(GPIOB, 1<<9, 4);

    I2C_InitStructure.I2C_ClockSpeed = 400000;              // 100K
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;              // I2C mode
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;      // 50% duty cycle --> standard
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;               // 自身地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C_Port,&I2C_InitStructure);
    I2C_Cmd(I2C_Port,ENABLE);
}

uint8_t Hard_IIC_Send_Byte(uint8_t Address,uint8_t Register,uint8_t Byte)
{
    uint16_t TimeOut = 10000;
    
    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB))  // 等待起始条件生成完毕
    {
        if (--TimeOut == 0)
        {
            return 1;
        }
    }

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Transmitter);
    while (!(I2C1->SR1 & I2C_SR1_ADDR))  // 等待地址发送完毕
    {
        if (--TimeOut == 0)
        {
            return 1;
        }
    }
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = Register;
    while (!(I2C1->SR1 & I2C_SR1_TXE))  // 等待数据寄存器空
    {
        if (--TimeOut == 0)
        {
            return 1;
        }
    }

    // 发送数据
    I2C1->DR = Byte;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;
    return 0;
}

uint8_t Hard_IIC_Wirter_Data(uint8_t Address,uint8_t Register,uint8_t *Data,uint8_t Count)
{
    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Transmitter);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = Register;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 发送数据
    for (uint16_t i = 0; i < Count; i++)
    {
        I2C1->DR = Data[i];
        while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空
    }

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;

    return 0;
}


uint8_t Hard_IIC_Receive_Byte(uint8_t Address,uint8_t Register)
{
    uint8_t Data;

    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Transmitter);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = Register;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Receiver);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 读取数据
    while (!(I2C1->SR1 & I2C_SR1_RXNE));  // 等待数据接收完毕
    Data = I2C1->DR;

    // 生成停止条件
    I2C1->CR1 |= I2C_CR1_STOP;

    return Data;

}

uint8_t Hard_IIC_Read_Data(uint8_t Address,uint8_t Register,uint8_t *Data,uint8_t Count)
{
    // 生成起始条件
    I2C_GenerateSTART(I2C2, ENABLE);
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Transmitter);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    // 发送寄存器地址
    I2C1->DR = Register;
    while (!(I2C1->SR1 & I2C_SR1_TXE));  // 等待数据寄存器空

    // 生成起始条件
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // 等待起始条件生成完毕

    // 发送从设备地址和写命令
    I2C_Send7bitAddress(I2C_Port, Address, I2C_Direction_Receiver);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));  // 等待地址发送完毕
    (void)I2C1->SR2;  // 清除ADDR标志

    int i=0;
    while(Count)
    {
        if(Count==1)
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);		//在接收最后一个字节之前提前将应答失能
            											//在接收最后一个字节之前提前申请停止条件
        }
        Count--;
        while (!(I2C1->SR1 & I2C_SR1_RXNE));  // 等待数据接收完毕
	    Data[i] = I2C_ReceiveData(I2C2);		//接收数据寄存器
        i++;
    }
    I2C_GenerateSTOP(I2C2, ENABLE);
	I2C_AcknowledgeConfig(I2C2, ENABLE);									//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return 0;
}

#endif