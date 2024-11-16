/**
 ****************************************************************************************************
 * @file        ft5206.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2022-12-27
 * @brief       7寸电容触摸屏-FT5206/FT5426 驱动代码
 * @license     Copyright (c) 2022-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20221227
 * 第一次发布
 * V1.1 20230607
 * 1，新增对ST7796 3.5寸屏 GT1151的支持
 * 2，新增对ILI9806 4.3寸屏 GT1151的支持
 *
 ****************************************************************************************************
 */

#ifndef __FT5206_H
#define __FT5206_H

#ifdef __LCD_GUI_

#include <gpio.h>
#include <sys.h>

/******************************************************************************************/
/* FT5206 INT 和 RST 引脚 定义 */

#define FT5206_RST_GPIO_PORT            GPIOI
#define FT5206_RST_GPIO_PIN             SYS_GPIO_PIN8
#define FT5206_RST_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* PI口时钟使能 */

#define FT5206_INT_GPIO_PORT            GPIOH
#define FT5206_INT_GPIO_PIN             SYS_GPIO_PIN7
#define FT5206_INT_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH口时钟使能 */

/******************************************************************************************/

/* 与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
 * IO操作函数 
 */
#define FT5206_RST(x)   bsp_gpio_pin_set(FT5206_RST_GPIO_PORT, FT5206_RST_GPIO_PIN, x)  /* 复位引脚 */
#define FT5206_INT      bsp_gpio_pin_get(FT5206_INT_GPIO_PORT, FT5206_INT_GPIO_PIN)     /* 读取做的引脚 */

/* IIC读写命令 */
#define FT5206_CMD_WR               0X70        /* 写命令(最低位为0) */
#define FT5206_CMD_RD               0X71        /* 读命令(最低位为1) */

/* FT5206 部分寄存器定义  */
#define FT5206_DEVIDE_MODE          0x00        /* FT5206模式控制寄存器 */
#define FT5206_REG_NUM_FINGER       0x02        /* 触摸状态寄存器 */

#define FT5206_TP1_REG              0X03        /* 第一个触摸点数据地址 */
#define FT5206_TP2_REG              0X09        /* 第二个触摸点数据地址 */
#define FT5206_TP3_REG              0X0F        /* 第三个触摸点数据地址 */
#define FT5206_TP4_REG              0X15        /* 第四个触摸点数据地址 */
#define FT5206_TP5_REG              0X1B        /* 第五个触摸点数据地址 */ 


#define	FT5206_ID_G_LIB_VERSION     0xA1        /* 版本 */
#define FT5206_ID_G_MODE            0xA4        /* FT5206中断模式控制寄存器 */
#define FT5206_ID_G_THGROUP         0x80        /* 触摸有效值设置寄存器 */
#define FT5206_ID_G_PERIODACTIVE    0x88        /* 激活状态周期设置寄存器 */


uint8_t ft5206_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void ft5206_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len);
uint8_t ft5206_init(void);
uint8_t ft5206_scan(uint8_t mode);


#endif

#endif

















