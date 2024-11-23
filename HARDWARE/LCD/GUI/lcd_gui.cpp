#include <lcd_gui.h>

/* 1-按下状态  0-放开状态  */
        
uint8_t button_act[10];


/// @brief 创建按键
/// @param act_sign 信号槽
/// @param button_num 按键号 1-10
/// @param x 按键起始x坐标
/// @param y 按键起始y坐标
/// @param w 按键长度
/// @param h 按键宽度
/// @param string 按键标签
void Button_check(uint8_t *act_sign,uint8_t button_num,uint16_t x,uint16_t y,uint16_t w,uint16_t h,char *string)     
{
    LCD_DrawRectangle(x,y,w,h);
    Printf(x+11,y+11,240,32,32,1,string);
    static int a=0;
    if(!button_act[button_num])  /* 检查按键状态  */
    {
        /* 处于放松状态  */
        if(tp_dev.x[0]>=x&&tp_dev.x[0]<=(x+w)&&tp_dev.y[0]>=y&&tp_dev.y[0]<=(y+h))
        {
            /* 按下  */
            *act_sign=button_num;    /* 发出槽信号  */

            button_act[button_num]=1;     /* 切换状态  */

            button_response(act_sign);
        }
    }    
    else
    {
        /* 仍然处于按下状态  */
                /* 停止响应  等待松开  */
        if(tp_dev.x[0]>=x&&tp_dev.x[0]<=(x+w)&&tp_dev.y[0]>=y&&tp_dev.y[0]<=(y+h))      
        {
                            /* 手没松开 退出继续等待  */
            if(!a)
            {
                LTDC_Fill(x+1,y+1,x+w-1,y+h-1,YELLOW);
                a=1;
            }
            /* 可拓展 长按--连点--功能  */
        }
        else                /* 若手松开 */
        {
            button_act[button_num]=0;
            a=0;
            *act_sign=-1;
            LTDC_Fill(x+1,y+1,x+w-1,y+h-1,WHITE);
        }
    }
}


void obj_move()
{
    static uint16_t x_move_sit=0,y_move_sit=0;
    while (y_move_sit<=650)
    {
        while (x_move_sit<=1025)
        {
            tp_dev.scan(0);
            //delay_ms(2);
            LCD_DrawLine(0+x_move_sit,0+y_move_sit,0+x_move_sit,50+y_move_sit);
            bsp_systick_delay_ms(1);
            //LCD_Clear(WHITE);
            x_move_sit++;
        }
        x_move_sit=0;
        y_move_sit+=50;
    }
    
}



/* 画板  */
void witer_line(void)
{
    uint8_t t = 0;
    uint8_t i = 0;
    uint16_t lastpos[10][2];                                                                                     /* ×îºóÒ»´ÎµÄÊý¾Ý */
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    while (1)
    {
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)                                    /* ×ø±êÔÚÆÁÄ»·¶Î§ÄÚ */
                {
                    if (lastpos[t][0] == 0xFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }

                    LCD_DrawLine(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t]); /* »­Ïß */
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];
                }
            }
            else 
            {
                lastpos[t][0] = 0xFFFF;
            }
        }

        bsp_systick_delay_ms(5);
        i++;

        if (i % 20 == 0){}
    }
}