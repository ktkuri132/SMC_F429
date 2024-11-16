/**
 ****************************************************************************************************
 * @file        touch.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2022-12-27
 * @brief       ������ ��������
 * @note        ֧�ֵ���/����ʽ������
 *              ������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206/GT1151�ȣ�����
 *
 * @license     Copyright (c) 2022-2032, ������������ӿƼ����޹�˾
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

#ifndef __TOUCH_H
#define __TOUCH_H


#ifdef __LCD_GUI_

#include "sys.h"
#include "gt9xxx.h"
#include "ft5206.h"
#include <sys.h>

/******************************************************************************************/
/* ���败��������IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK ���� ���� */

#define T_PEN_GPIO_PORT                 GPIOH
#define T_PEN_GPIO_PIN                  SYS_GPIO_PIN7
#define T_PEN_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_CS_GPIO_PORT                  GPIOI
#define T_CS_GPIO_PIN                   SYS_GPIO_PIN8
#define T_CS_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_MISO_GPIO_PORT                GPIOG
#define T_MISO_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MISO_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 6; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_MOSI_GPIO_PORT                GPIOI
#define T_MOSI_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MOSI_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* ����IO��ʱ��ʹ�� */

#define T_CLK_GPIO_PORT                 GPIOH
#define T_CLK_GPIO_PIN                  SYS_GPIO_PIN6
#define T_CLK_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* ����IO��ʱ��ʹ�� */

#define lcd_draw_line(x1,y1,x2,y2) LCD_DrawLine(x1,y1,x2,y2)
#define lcd_draw_point(x,y) LCD_DrawPoint(x,y)      
#define lcd_draw_circle(x,y,r) LCD_Draw_Circle(x,y,r)
/******************************************************************************************/

/* ���败������������ */
#define T_PEN           bsp_gpio_pin_get(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)           /* T_PEN */
#define T_MISO          bsp_gpio_pin_get(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)         /* T_MISO */

#define T_MOSI(x)       bsp_gpio_pin_set(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, x)      /* T_MOSI */
#define T_CLK(x)        bsp_gpio_pin_set(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN, x)        /* T_CLK */
#define T_CS(x)         bsp_gpio_pin_set(T_CS_GPIO_PORT, T_CS_GPIO_PIN, x)          /* T_CS */


#define TP_PRES_DOWN    0x8000      /* ���������� */
#define TP_CATH_PRES    0x4000      /* �а��������� */
#define CT_MAX_TOUCH    10          /* ������֧�ֵĵ���,�̶�Ϊ5�� */

/* ������������ */
typedef struct
{
    uint8_t (*init)(void);      /* ��ʼ�������������� */
    uint8_t (*scan)(uint8_t);   /* ɨ�败����.0,��Ļɨ��;1,��������; */
    void (*adjust)(void);       /* ������У׼ */
    uint16_t x[CT_MAX_TOUCH];   /* ��ǰ���� */
    uint16_t y[CT_MAX_TOUCH];   /* �����������10������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
                                 * x[9],y[9]�洢��һ�ΰ���ʱ������.
                                 */

    uint16_t sta;               /* �ʵ�״̬
                                 * b15:����1/�ɿ�0;
                                 * b14:0,û�а�������;1,�а�������.
                                 * b13~b10:����
                                 * b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
                                 */

    /* 5��У׼������У׼����(����������ҪУ׼) */
    float xfac;                 /* 5��У׼��x����������� */
    float yfac;                 /* 5��У׼��y����������� */
    short xc;                   /* ����X��������ֵ(ADֵ) */
    short yc;                   /* ����Y��������ֵ(ADֵ) */

    /* �����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
     * b0:0, ����(�ʺ�����ΪX����,����ΪY�����TP)
     *    1, ����(�ʺ�����ΪY����,����ΪX�����TP)
     * b1~6: ����.
     * b7:0, ������
     *    1, ������
     */
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;        /* ������������touch.c���涨�� */


/* ���������� */

static void tp_write_byte(uint8_t data);                /* �����оƬд��һ������ */
static uint16_t tp_read_ad(uint8_t cmd);                /* ��ȡADת��ֵ */
static uint16_t tp_read_xoy(uint8_t cmd);               /* ���˲��������ȡ(X/Y) */
void tp_read_xy(uint16_t *x, uint16_t *y);       /* ˫�����ȡ(X+Y) */
static uint8_t tp_read_xy2(uint16_t *x, uint16_t *y);   /* ����ǿ�˲���˫���������ȡ */

uint8_t tp_init(void);              /* ��ʼ�� */
uint8_t tp_scan(uint8_t mode);      /* ɨ�� */


#endif

#endif

















