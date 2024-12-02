#ifndef __RTOS_TASK_CONFIG_H
#define __RTOS_TASK_CONFIG_H
#include <FreeRTOS.h>
#include <Project.h>    
#define SDRAM_ADDR 0xC0000000
// 设置任务堆栈大小  单位:字节
#define TASK_STACK_SIZE (size_t)(0X2000000)// 32*1024*1024
// 设置栈顶指针
#define TASK_START_TOPADDR (StackType_t)(SDRAM_ADDR + TASK_STACK_SIZE)
// 设置任务栈
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__((section(".ext_sram")));

// 任务函数
void Task1_BSP_Init();
void Task2_Project_Init();
void Task3_Project_Display();
void Task4_sysControl();


#endif