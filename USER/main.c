//#define __RELEASE
#define __DEBUG

#if defined (__RELEASE)

#include <Project.h>


/// @brief 主函数运行完了自动复位
void BSP_Init()
{
    SDRAM_Init();
    bsp_usart_1_inti(115200);
    

}



#elif defined(__DEBUG)


#include <Project.h>

#include <inv_mpu.h>


extern uint8_t delay_time[15];

void usart_send_test();
void led_test();
void usart_receive_test();
void OLED_test();
void LCD_test();
void TIM2_Init(uint32_t ms);
//void Reset_Handler();
float pitch,roll,yaw;
int a;
/// @brief 主函数运行完了自动复位
void BSP_Init()
{
    

    SDRAM_Init();
    bsp_usart_1_inti(115200);
    // 把PB7当VCC用,but other pin can't be used to GND
    bsp_gpio_init(GPIOB, SYS_GPIO_PIN7, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_PU);
    bsp_gpio_pin_set(GPIOB, SYS_GPIO_PIN7, 1);
    

    // 创建动态任务
    //xTaskCreate((TaskFunction_t)StartTask, "StartTask", 1024, NULL, 1, &StartTask_Handler);
    //vTaskStartScheduler();
    // 软件复位
    //Reset_Handler();
    Project_BSP_PWM_TIM2_Init();
    Project_BSP_TB6612_Init();
    AllForward
    OLED_Init();
    OLED_Clear();
    int b;
    b = mpu_dmp_init();
    // while(b)
    // {
    //     b = mpu_dmp_init();
    // }

    // mpu_dmp_get_data(&pitch,&roll,&yaw);
    
    // delay_time[10] = 0;
    // delay_time[1] = 0;
    // delay_time[12] = 0;
    // delay_time[3] = 0;


    
    // TIM2_Init(8);
    while (1)
    {
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        printf("%d,%f\r\n",b,pitch);
        OLED_Printf(0, 0,  OLED_8X16,"%f",pitch);
        OLED_Printf(0, 16, OLED_8X16,"%f",roll);
        OLED_Printf(0, 32, OLED_8X16,"%f",yaw);
        OLED_Printf(0, 48, OLED_8X16,"%d",++a);
        OLED_Update();
    }
   
    // 串口发送测试
    //usart_send_test();
    // 串口接收测试
    //usart_receive_test();
    // LED测试
    //led_test();
    // OLED测试
    //OLED_test();
    // MPU6050测试
    //MPU6050_test();
    // LCD测试
    //LCD_test();
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        a=0;
        
    }
}

//定时器2初始化
void TIM2_Init(uint32_t ms)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 1000*ms - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    NVIC_Configuration();
}

void OLED_test()
{
    // OLED初始化
    OLED_Init();
    OLED_Clear();
    
    while (1)
    {
        // OLED显示
        OLED_Printf(0, 0, OLED_8X16,"hello,world");
        // OLED更新
        OLED_Update();
    }
}

void usart_receive_test()
{
    // 中断初始化
    //NVIC_Configuration();
    // 串口初始化
    bsp_usart_1_inti(115200);
    int hour=0,min=0,sec=0;
    while (1)
    {   


        printf("%d:%d:%d\r\n",hour,min,sec++);
        vTaskDelay(1000);
        if(sec==60)
        {
            sec=0;
            min++;
            if(min==60)
            {
                min=0;
                hour++;
                if(hour==24)
                {
                    hour=0;
                }
            }
        }
    }
}

void usart_send_test()
{
    // 串口初始化
    bsp_usart_1_inti(115200);
    while (1)
    {
        printf("hello world!\r\n");
        bsp_systick_delay_ms(1000);
    }
}

void led_test()
{

    // 使能GPIOB时钟
    RCC->AHB1ENR |= 1 << 1;
    bsp_gpio_init(GPIOB, SYS_GPIO_PIN0, SYS_GPIO_MODE_OUT, SYS_GPIO_OTYPE_PP, SYS_GPIO_SPEED_HIGH, SYS_GPIO_PUPD_NONE);
    
    while (1)
    {
        bsp_gpio_pin_set(GPIOB, SYS_GPIO_PIN0, 0);
        vTaskDelay(500);
        //bsp_systick_delay_ms(500);
        bsp_gpio_pin_set(GPIOB, SYS_GPIO_PIN0, 1);
        vTaskDelay(500);
        //bsp_systick_delay_ms(500);
    }
    
}

#endif