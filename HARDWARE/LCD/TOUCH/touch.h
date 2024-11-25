/**
 ****************************************************************************************************
 * @file        touch.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2022-12-27
 * @brief       触摸屏 驱动代码
 * @note        支持电阻/电容式触摸屏
 *              触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206/GT1151等）代码
 *
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

#ifndef __TOUCH_H
#define __TOUCH_H


#ifdef __LCD_GUI_

#include "sys.h"
#include "gt9xxx.h"
#include "ft5206.h"
#include <sys.h>

/******************************************************************************************/
/* 电阻触摸屏驱动IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK 引脚 定义 */

#define T_PEN_GPIO_PORT                 GPIOH
#define T_PEN_GPIO_PIN                  SYS_GPIO_PIN7
#define T_PEN_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* 所在IO口时钟使能 */

#define T_CS_GPIO_PORT                  GPIOI
#define T_CS_GPIO_PIN                   SYS_GPIO_PIN8
#define T_CS_GPIO_CLK_ENABLE()          do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* 所在IO口时钟使能 */

#define T_MISO_GPIO_PORT                GPIOG
#define T_MISO_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MISO_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 6; }while(0)   /* 所在IO口时钟使能 */

#define T_MOSI_GPIO_PORT                GPIOI
#define T_MOSI_GPIO_PIN                 SYS_GPIO_PIN3
#define T_MOSI_GPIO_CLK_ENABLE()        do{ RCC->AHB1ENR |= 1 << 8; }while(0)   /* 所在IO口时钟使能 */

#define T_CLK_GPIO_PORT                 GPIOH
#define T_CLK_GPIO_PIN                  SYS_GPIO_PIN6
#define T_CLK_GPIO_CLK_ENABLE()         do{ RCC->AHB1ENR |= 1 << 7; }while(0)   /* 所在IO口时钟使能 */

#define lcd_draw_line(x1,y1,x2,y2) LCD_DrawLine(x1,y1,x2,y2)
#define lcd_draw_point(x,y) LCD_DrawPoint(x,y)      
#define lcd_draw_circle(x,y,r) LCD_Draw_Circle(x,y,r)
/******************************************************************************************/

/* 电阻触摸屏控制引脚 */
#define T_PEN           bsp_gpio_pin_get(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)           /* T_PEN */
#define T_MISO          bsp_gpio_pin_get(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)         /* T_MISO */

#define T_MOSI(x)       bsp_gpio_pin_set(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, x)      /* T_MOSI */
#define T_CLK(x)        bsp_gpio_pin_set(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN, x)        /* T_CLK */
#define T_CS(x)         bsp_gpio_pin_set(T_CS_GPIO_PORT, T_CS_GPIO_PIN, x)          /* T_CS */


#define TP_PRES_DOWN    0x8000      /* 触屏被按下 */
#define TP_CATH_PRES    0x4000      /* 有按键按下了 */
#define CT_MAX_TOUCH    10          /* 电容屏支持的点数,固定为5点 */

/* 触摸屏控制器 */
typedef struct
{
    uint8_t (*init)(void);      /* 初始化触摸屏控制器 */
    uint8_t (*scan)(uint8_t);   /* 扫描触摸屏.0,屏幕扫描;1,物理坐标; */
    void (*adjust)(void);       /* 触摸屏校准 */
    uint16_t x[CT_MAX_TOUCH];   /* 当前坐标 */
    uint16_t y[CT_MAX_TOUCH];   /* 电容屏有最多10组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
                                 * x[9],y[9]存储第一次按下时的坐标.
                                 */

    uint16_t sta;               /* 笔的状态
                                 * b15:按下1/松开0;
                                 * b14:0,没有按键按下;1,有按键按下.
                                 * b13~b10:保留
                                 * b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
                                 */

    /* 5点校准触摸屏校准参数(电容屏不需要校准) */
    float xfac;                 /* 5点校准法x方向比例因子 */
    float yfac;                 /* 5点校准法y方向比例因子 */
    short xc;                   /* 中心X坐标物理值(AD值) */
    short yc;                   /* 中心Y坐标物理值(AD值) */

    /* 新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
     * b0:0, 竖屏(适合左右为X坐标,上下为Y坐标的TP)
     *    1, 横屏(适合左右为Y坐标,上下为X坐标的TP)
     * b1~6: 保留.
     * b7:0, 电阻屏
     *    1, 电容屏
     */
    uint8_t touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;        /* 触屏控制器在touch.c里面定义 */


/* 电阻屏函数 */

static void tp_write_byte(uint8_t data);                /* 向控制芯片写入一个数据 */
static uint16_t tp_read_ad(uint8_t cmd);                /* 读取AD转换值 */
static uint16_t tp_read_xoy(uint8_t cmd);               /* 带滤波的坐标读取(X/Y) */
void tp_read_xy(uint16_t *x, uint16_t *y);       /* 双方向读取(X+Y) */
static uint8_t tp_read_xy2(uint16_t *x, uint16_t *y);   /* 带加强滤波的双方向坐标读取 */

uint8_t tp_init(void);              /* 初始化 */
uint8_t tp_scan(uint8_t mode);      /* 扫描 */


#endif

#endif

















