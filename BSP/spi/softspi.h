#ifndef On_Chip_SPI
#define On_Chip_SPI

#ifndef  __SOFTSPI_
#define  __SOFTSPI_


#include <gpio/gpio.h>
// #include "gpio.h"

// SPI协议通用接口
#define SOFT_SPI_SCK_PORT   GPIOF
#define SOFT_SPI_MOSI_PORT  GPIOF
#define SOFT_SPI_MISO_PORT  GPIOF
#define SOFT_SPI_CS_PORT    GPIOF

#define SOFT_SPI_SCK_PIN    SYS_GPIO_PIN7
#define SOFT_SPI_MOSI_PIN   SYS_GPIO_PIN9
#define SOFT_SPI_MISO_PIN   SYS_GPIO_PIN8
#define SOFT_SPI_CS_PIN     SYS_GPIO_PIN6

#define Soft_SPI_W_CS(x)     GPIO_WriteBit(SOFT_SPI_CS_PORT,SOFT_SPI_CS_PIN,x)
#define Soft_SPI_W_SCK(x)    GPIO_WriteBit(SOFT_SPI_SCK_PORT,SOFT_SPI_SCK_PIN,x)
#define Soft_SPI_W_MOSI(x)   GPIO_WriteBit(SOFT_SPI_MOSI_PORT,SOFT_SPI_MOSI_PIN,x)
#define Soft_SPI_R_MISO()    GPIO_ReadInputDataBit(SOFT_SPI_MISO_PORT,SOFT_SPI_MISO_PIN)

void Soft_SPI_Init(void);
void Soft_SPI_SendByte(uint8_t CS,uint8_t Byte);
void Soft_SPI_SendData(uint8_t CS,uint8_t *Data,uint8_t Count);
uint8_t Soft_SPI_ReceiveByte(uint8_t CS);
void Soft_SPI_ReceiveData(uint8_t CS,uint8_t *Data,uint8_t Count);
uint8_t Soft_SPI_SwapByte(uint8_t SData);


#endif
#endif