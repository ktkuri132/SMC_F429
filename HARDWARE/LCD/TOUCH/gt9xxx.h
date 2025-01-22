/**
 ****************************************************************************************************
 * @file        gt9xxx.h
 * @author      锟斤拷锟斤拷原锟斤拷锟脚讹拷(ALIENTEK)
 * @version     V1.1
 * @date        2022-12-27
 * @brief       4.3锟斤拷锟斤拷荽锟斤拷锟斤拷锟�-GT9xxx 锟斤拷锟斤拷锟斤拷锟斤拷
 *   @note      GT系锟叫碉拷锟捷达拷锟斤拷锟斤拷IC通锟斤拷锟斤拷锟斤拷,锟斤拷锟斤拷锟斤拷支锟斤拷: GT9147/GT917S/GT968/GT1151/GT9271 锟饺讹拷锟斤拷
 *              锟斤拷锟斤拷IC, 锟斤拷些锟斤拷锟斤拷IC锟斤拷ID锟斤拷一锟斤拷, 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷要锟斤拷锟轿猴拷锟睫改硷拷锟斤拷通锟斤拷锟斤拷锟斤拷锟斤拷直锟斤拷锟斤拷锟斤拷
 *
 * @license     Copyright (c) 2022-2032, 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟接科硷拷锟斤拷锟睫癸拷司
 ****************************************************************************************************
 * @attention
 *
 * 实锟斤拷平台:锟斤拷锟斤拷原锟斤拷 锟斤拷锟斤拷锟斤拷 F429锟斤拷锟斤拷锟斤拷
 * 锟斤拷锟斤拷锟斤拷频:www.yuanzige.com
 * 锟斤拷锟斤拷锟斤拷坛:www.openedv.com
 * 锟斤拷司锟斤拷址:www.alientek.com
 * 锟斤拷锟斤拷锟街�:openedv.taobao.com
 *
 * 锟睫革拷说锟斤拷
 * V1.0 20221227
 * 锟斤拷一锟轿凤拷锟斤拷
 * V1.1 20230607
 * 1锟斤拷锟斤拷锟斤拷锟斤拷ST7796 3.5锟斤拷锟斤拷 GT1151锟斤拷支锟斤拷
 * 2锟斤拷锟斤拷锟斤拷锟斤拷ILI9806 4.3锟斤拷锟斤拷 GT1151锟斤拷支锟斤拷
 *
 ****************************************************************************************************
 */

#ifndef __GT9XXX_H
#define __GT9XXX_H


#ifdef __LCD_GUI_

#include "sys.h"

/******************************************************************************************/
/* GT9XXX INT 锟斤拷 RST 锟斤拷锟斤拷 锟斤拷锟斤拷 */

#define GT9XXX_RST_GPIO_PORT            GPIOI
#define GT9XXX_RST_GPIO_PIN             SYS_GPIO_PIN8
#define GT9XXX_RST_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* PI锟斤拷时锟斤拷使锟斤拷 */

#define GT9XXX_INT_GPIO_PORT            GPIOH
#define GT9XXX_INT_GPIO_PIN             SYS_GPIO_PIN7
#define GT9XXX_INT_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH锟斤拷时锟斤拷使锟斤拷 */

/******************************************************************************************/

/* 锟斤拷锟斤拷荽锟斤拷锟斤拷锟斤拷锟斤拷拥锟叫酒拷锟斤拷锟�(未锟斤拷锟斤拷IIC锟斤拷锟斤拷) 
 * IO锟斤拷锟斤拷锟斤拷锟斤拷 
 */
#define GT9XXX_RST(x)   bsp_gpio_pin_set(GT9XXX_RST_GPIO_PORT, GT9XXX_RST_GPIO_PIN, x)  /* 锟斤拷位锟斤拷锟斤拷 */
#define GT9XXX_INT      bsp_gpio_pin_get(GT9XXX_INT_GPIO_PORT, GT9XXX_INT_GPIO_PIN)     /* 锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷 */

 
/* IIC锟斤拷写锟斤拷锟斤拷 */
#define GT9XXX_CMD_WR       0X28        /* 写锟斤拷锟斤拷 */
#define GT9XXX_CMD_RD       0X29        /* 锟斤拷锟斤拷锟斤拷 */

/* GT9XXX 锟斤拷锟街寄达拷锟斤拷锟斤拷锟斤拷  */
#define GT9XXX_CTRL_REG     0X8040      /* GT9XXX锟斤拷锟狡寄达拷锟斤拷 */
#define GT9XXX_CFGS_REG     0X8047      /* GT9XXX锟斤拷锟斤拷锟斤拷始锟斤拷址锟侥达拷锟斤拷 */
#define GT9XXX_CHECK_REG    0X80FF      /* GT9XXX校锟斤拷图拇锟斤拷锟� */
#define GT9XXX_PID_REG      0X8140      /* GT9XXX锟斤拷品ID锟侥达拷锟斤拷 */

#define GT9XXX_GSTID_REG    0X814E      /* GT9XXX锟斤拷前锟斤拷獾斤拷拇锟斤拷锟斤拷锟斤拷 */
#define GT9XXX_TP1_REG      0X8150      /* 锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP2_REG      0X8158      /* 锟节讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP3_REG      0X8160      /* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP4_REG      0X8168      /* 锟斤拷锟侥革拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP5_REG      0X8170      /* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷莸锟街� */
#define GT9XXX_TP6_REG      0X8178      /* 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP7_REG      0X8180      /* 锟斤拷锟竭革拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP8_REG      0X8188      /* 锟节八革拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP9_REG      0X8190      /* 锟节九革拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
#define GT9XXX_TP10_REG     0X8198      /* 锟斤拷十锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷碉拷址 */
 

uint8_t gt9xxx_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void gt9xxx_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len); 
uint8_t gt9xxx_init(void);
uint8_t gt9xxx_scan(uint8_t mode);
void Read_SitData(uint8_t i);



#endif






#endif













