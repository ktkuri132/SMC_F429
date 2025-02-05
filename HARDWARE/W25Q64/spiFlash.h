//
// Created by ktkuri on 25-2-5.
//

#ifndef SPIFLASH_H
#define SPIFLASH_H

#include <spi/softspi.h>
#include <gpio/gpio.h>
#include <sys.h>

#define QCS SOFT_SPI_CS1_PIN
#define QMISO SOFT_SPI_MISO_PIN
#define QMOSI SOFT_SPI_MOSI_PIN
#define QSCK SOFT_SPI_SCK_PIN

#define QHOLD_Port GPIOF
#define QHOLD SYS_GPIO_PIN6

#define QWP_Port GPIOF
#define QWP SYS_GPIO_PIN7

#define W25Q64_GPIO_Init() Soft_SPI_Init()

#endif //SPIFLASH_H
