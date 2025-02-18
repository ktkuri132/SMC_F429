#ifndef __RTOS_TASK_CONFIG_H
#define __RTOS_TASK_CONFIG_H
#include <RTOS/comment//FreeRTOS.h>
#include <Project/Project.h>
#define SDRAM_ADDR 0xC0000000
// 设置任务堆栈大小  单位:字节
#define TASK_STACK_SIZE (size_t)(0X2000000)// 32*1024*1024
// 设置栈顶指针
#define TASK_START_TOPADDR (StackType_t)(SDRAM_ADDR + TASK_STACK_SIZE)

// 任务函数
void Task1_SystemStrat(uint8_t Mode);
void Task_DebugLog();

void Task2_Project_Init();
void Task3_Project_Display(uint8_t Mode);
void Task4_LEDPlay(uint8_t Mode);
void Task5_KeyScan();


#endif