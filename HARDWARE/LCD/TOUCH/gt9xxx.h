/**
 ****************************************************************************************************
 * @file        gt9xxx.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2022-12-27
 * @brief       4.3����ݴ�����-GT9xxx ��������
 *   @note      GTϵ�е��ݴ�����ICͨ������,������֧��: GT9147/GT917S/GT968/GT1151/GT9271 �ȶ���
 *              ����IC, ��Щ����IC��ID��һ��, ��������������Ҫ���κ��޸ļ���ͨ��������ֱ������
 *
 * @license     Copyright (c) 2022-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20221227
 * ��һ�η���
 * V1.1 20230607
 * 1��������ST7796 3.5���� GT1151��֧��
 * 2��������ILI9806 4.3���� GT1151��֧��
 *
 ****************************************************************************************************
 */

#ifndef __GT9XXX_H
#define __GT9XXX_H


#ifdef __LCD_GUI_

#include <sys.h>

/******************************************************************************************/
/* GT9XXX INT �� RST ���� ���� */

#define GT9XXX_RST_GPIO_PORT            GPIOI
#define GT9XXX_RST_GPIO_PIN             SYS_GPIO_PIN8
#define GT9XXX_RST_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* PI��ʱ��ʹ�� */

#define GT9XXX_INT_GPIO_PORT            GPIOH
#define GT9XXX_INT_GPIO_PIN             SYS_GPIO_PIN7
#define GT9XXX_INT_GPIO_CLK_ENABLE()    do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* PH��ʱ��ʹ�� */

/******************************************************************************************/

/* ����ݴ��������ӵ�оƬ����(δ����IIC����) 
 * IO�������� 
 */
#define GT9XXX_RST(x)   bsp_gpio_pin_set(GT9XXX_RST_GPIO_PORT, GT9XXX_RST_GPIO_PIN, x)  /* ��λ���� */
#define GT9XXX_INT      bsp_gpio_pin_get(GT9XXX_INT_GPIO_PORT, GT9XXX_INT_GPIO_PIN)     /* ��ȡ�������� */

 
/* IIC��д���� */
#define GT9XXX_CMD_WR       0X28        /* д���� */
#define GT9XXX_CMD_RD       0X29        /* ������ */

/* GT9XXX ���ּĴ�������  */
#define GT9XXX_CTRL_REG     0X8040      /* GT9XXX���ƼĴ��� */
#define GT9XXX_CFGS_REG     0X8047      /* GT9XXX������ʼ��ַ�Ĵ��� */
#define GT9XXX_CHECK_REG    0X80FF      /* GT9XXXУ��ͼĴ��� */
#define GT9XXX_PID_REG      0X8140      /* GT9XXX��ƷID�Ĵ��� */

#define GT9XXX_GSTID_REG    0X814E      /* GT9XXX��ǰ��⵽�Ĵ������ */
#define GT9XXX_TP1_REG      0X8150      /* ��һ�����������ݵ�ַ */
#define GT9XXX_TP2_REG      0X8158      /* �ڶ������������ݵ�ַ */
#define GT9XXX_TP3_REG      0X8160      /* ���������������ݵ�ַ */
#define GT9XXX_TP4_REG      0X8168      /* ���ĸ����������ݵ�ַ */
#define GT9XXX_TP5_REG      0X8170      /* ��������������ݵ�ַ */
#define GT9XXX_TP6_REG      0X8178      /* ���������������ݵ�ַ */
#define GT9XXX_TP7_REG      0X8180      /* ���߸����������ݵ�ַ */
#define GT9XXX_TP8_REG      0X8188      /* �ڰ˸����������ݵ�ַ */
#define GT9XXX_TP9_REG      0X8190      /* �ھŸ����������ݵ�ַ */
#define GT9XXX_TP10_REG     0X8198      /* ��ʮ�����������ݵ�ַ */
 

uint8_t gt9xxx_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);
void gt9xxx_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len); 
uint8_t gt9xxx_init(void);
uint8_t gt9xxx_scan(uint8_t mode);
void Read_SitData(uint8_t i);



#endif






#endif













