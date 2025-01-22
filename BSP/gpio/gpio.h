#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)   

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)   

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)   

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)   

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)   
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n) 

/* sys_nvic_ex_config专用宏定义 */
#define SYS_GPIO_FTIR               1       /* 下降沿触发 */
#define SYS_GPIO_RTIR               2       /* 上升沿触发 */
#define SYS_GPIO_BTIR               3       /* 任意边沿触发 */

/* GPIO设置专用宏定义 */
#define SYS_GPIO_MODE_IN            0       /* 普通输入模式 */
#define SYS_GPIO_MODE_OUT           1       /* 普通输出模式 */
#define SYS_GPIO_MODE_AF            2       /* AF功能模式 */
#define SYS_GPIO_MODE_AIN           3       /* 模拟输入模式 */

#define SYS_GPIO_SPEED_LOW          0       /* GPIO速度(低速,2M) */
#define SYS_GPIO_SPEED_MID          1       /* GPIO速度(中速,25M) */
#define SYS_GPIO_SPEED_FAST         2       /* GPIO速度(快速,50M) */
#define SYS_GPIO_SPEED_HIGH         3       /* GPIO速度(高速,100M) */

#define SYS_GPIO_PUPD_NONE          0       /* 不带上下拉 */
#define SYS_GPIO_PUPD_PU            1       /* 上拉 */
#define SYS_GPIO_PUPD_PD            2       /* 下拉 */
#define SYS_GPIO_PUPD_RES           3       /* 保留 */

#define SYS_GPIO_OTYPE_PP           0       /* 推挽输出 */
#define SYS_GPIO_OTYPE_OD           1       /* 开漏输出 */

/* GPIO引脚位置宏定义  */
#define SYS_GPIO_PIN0               1<<0
#define SYS_GPIO_PIN1               1<<1
#define SYS_GPIO_PIN2               1<<2
#define SYS_GPIO_PIN3               1<<3
#define SYS_GPIO_PIN4               1<<4
#define SYS_GPIO_PIN5               1<<5
#define SYS_GPIO_PIN6               1<<6
#define SYS_GPIO_PIN7               1<<7
#define SYS_GPIO_PIN8               1<<8
#define SYS_GPIO_PIN9               1<<9
#define SYS_GPIO_PIN10              1<<10
#define SYS_GPIO_PIN11              1<<11
#define SYS_GPIO_PIN12              1<<12
#define SYS_GPIO_PIN13              1<<13
#define SYS_GPIO_PIN14              1<<14
#define SYS_GPIO_PIN15              1<<15

#define PIN0                        0
#define PIN1                        1
#define PIN2                        2
#define PIN3                        3
#define PIN4                        4
#define PIN5                        5
#define PIN6                        6
#define PIN7                        7
#define PIN8                        8
#define PIN9                        9
#define PIN10                       10
#define PIN11                       11
#define PIN12                       12
#define PIN13                       13
#define PIN14                       14
#define PIN15                       15


void bsp_gpio_init(GPIO_TypeDef *GPIOx, uint32_t PINx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd);
void bsp_gpio_af_set(GPIO_TypeDef *GPIOx, uint16_t PINx, uint8_t AFx);
void bsp_gpio_pin_set(GPIO_TypeDef *GPIOx, uint16_t PINx, uint8_t status);
uint8_t bsp_gpio_pin_get(GPIO_TypeDef *GPIOx, uint16_t PINx);




#endif