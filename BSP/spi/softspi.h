#ifndef On_Chip_SPI
#define On_Chip_SPI

#ifndef  __SOFTSPI_
#define  __SOFTSPI_

#include "sys.h"


// SPI协议通用接口
#define SOFT_SPI_SCK_PORT   GPIOB
#define SOFT_SPI_MOSI_PORT  GPIOB
#define SOFT_SPI_MISO_PORT  GPIOB
#define SOFT_SPI_CS1_PORT   GPIOB
#define SOFT_SPI_CS2_PORT   GPIOB
#define SOFT_SPI_CS3_PORT   GPIOB

#define SOFT_SPI_SCK_PIN    SYS_GPIO_PIN8
#define SOFT_SPI_MOSI_PIN   SYS_GPIO_PIN9
#define SOFT_SPI_MISO_PIN   0

#define SOFT_SPI_CS1_PIN    SYS_GPIO_PIN11
#define SOFT_SPI_CS2_PIN    0
#define SOFT_SPI_CS3_PIN    0

// 部分SPI外设的兼容接口
// #define SOFT_SPI_DC_PORT  GPIOB
// #define SOFT_SPI_DC_PIN   SYS_GPIO_PIN10

// #define SOFT_SPI_RES_PORT GPIOB
// #define SOFT_SPI_RES_PIN  SYS_GPIO_PIN7


void Soft_SPI_Init(void);
void Soft_SPI_SendByte(uint8_t CS,uint8_t Byte);
void Soft_SPI_SendData(uint8_t CS,uint8_t *Data,uint8_t Count);
uint8_t Soft_SPI_ReceiveByte(uint8_t CS);
void Soft_SPI_ReceiveData(uint8_t CS,uint8_t *Data,uint8_t Count);
void Soft_SwapData(uint8_t CS,uint8_t SData);


#endif
#endif