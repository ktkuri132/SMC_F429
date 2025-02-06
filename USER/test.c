//
// Created by 34575 on 25-2-5.
//
#include <stdio.h>
#include <W25Q64/spiFlash.h>
void Test(){

  W25Q64_Init();

  uint8_t  MID;
  uint16_t DID;
  uint8_t DataArry[1] = {0x78};
  W25Q64_ReadID(&MID,&DID);
  // W25Q64_ReadData(0x00000000,&MID,1);
  printf("MID:%d,DID:%d\n",MID,DID);
  // W25Q64_PageProgram(0x00000000,DataArry,1);
  W25Q64_ReadData(0x00000000,&MID,1);
  printf("MID:%x,DID:%d\n",MID,DID);
  while(1){
    bsp_systick_delay_ms(1000);
    // printf("MID:%d,DID:%d\n",MID,DID);
  }
}
