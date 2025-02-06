//
// Created by ktkuri on 25-2-5.
//

#ifndef SPIFLASH_H
#define SPIFLASH_H

#include <spi/softspi.h>
#include <gpio/gpio.h>
#include <sys.h>

#define W25Q64_WRITE_ENABLE							0x06    //写使能
#define W25Q64_WRITE_DISABLE						0x04    //写禁止
#define W25Q64_READ_STATUS_REGISTER_1				0x05    //读状态寄存器1
#define W25Q64_READ_STATUS_REGISTER_2				0x35    //读状态寄存器2
#define W25Q64_WRITE_STATUS_REGISTER				0x01    //写状态寄存器
#define W25Q64_PAGE_PROGRAM							0x02    //页编程
#define W25Q64_QUAD_PAGE_PROGRAM					0x32    //四字节页编程
#define W25Q64_BLOCK_ERASE_64KB						0xD8    //64KB块擦除
#define W25Q64_BLOCK_ERASE_32KB						0x52    //32KB块擦除
#define W25Q64_SECTOR_ERASE_4KB						0x20    //4KB扇区擦除
#define W25Q64_CHIP_ERASE							0xC7    //整片擦除
#define W25Q64_ERASE_SUSPEND						0x75    //擦除暂停
#define W25Q64_ERASE_RESUME							0x7A    //擦除恢复
#define W25Q64_POWER_DOWN							0xB9    //掉电
#define W25Q64_HIGH_PERFORMANCE_MODE				0xA3    //高性能模式
#define W25Q64_CONTINUOUS_READ_MODE_RESET			0xFF    //连续读模式复位
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB    //释放掉电和高性能模式的设备ID
#define W25Q64_MANUFACTURER_DEVICE_ID				0x90    //制造商和设备ID
#define W25Q64_READ_UNIQUE_ID						0x4B    //读唯一ID
#define W25Q64_JEDEC_ID								0x9F    //JEDEC ID
#define W25Q64_READ_DATA							0x03    //读数据
#define W25Q64_FAST_READ							0x0B    //快速读
#define W25Q64_FAST_READ_DUAL_OUTPUT				0x3B    //快速读双输出
#define W25Q64_FAST_READ_DUAL_IO					0xBB    //快速读双IO
#define W25Q64_FAST_READ_QUAD_OUTPUT				0x6B    //快速读四输出
#define W25Q64_FAST_READ_QUAD_IO					0xEB    //快速读四IO
#define W25Q64_OCTAL_WORD_READ_QUAD_IO				0xE3    //八字节读四IO
#define W25Q64_DUMMY_BYTE							0xFF    //空字节

#define Soft_SPI_Start()  Soft_SPI_W_CS(0)
#define Soft_SPI_Stop()   Soft_SPI_W_CS(1)


void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);


#endif //SPIFLASH_H
