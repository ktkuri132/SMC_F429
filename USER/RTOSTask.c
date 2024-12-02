#include <Project.h>

extern Stde_DataTypeDef USART3_DataBuff,UART5_DataBuff,UART4_DataBuff;

void Task1_BSP_Init()
{
    
    bsp_usart_1_inti(115200);   // 产生调试信息
    bsp_usart_2_inti(9600);     // 蓝牙串口
    bsp_usart_3_inti(115200);   // OpenMV摄像头通信
    bsp_uart_4_inti(115200);    // 无线串口
    bsp_uart_5_inti(115200);    // V831摄像头通信
    OLED_Init();
    mpu_dmp_init();
    NVIC_Configuration();
}

void Task2_Project_Init()
{
    // 初始化串口数据结构
    Stde_DataTypeDef_Init(&USART3_DataBuff);
    Stde_DataTypeDef_Init(&UART5_DataBuff);
    Stde_DataTypeDef_Init(&UART4_DataBuff);

    // Project_BSP_PWM_TIM2_Init();   printf("初始化PWM\n");  
    // Project_BSP_Encoding_Init();   printf("初始化编码器");
    // Project_BSP_TB6612_Init();     printf("初始化TB6612\n");  
    // Project_BSP_HW201_Init();      printf("初始化红外传感器");
}

float pitch,roll,yaw;


void Task3_Project_Display()
{
    while(1)
    {
        mpu_dmp_get_data(&pitch,&roll,&yaw);    
        OLED_Printf(0,0,OLED_8X16,"OpenMV:%d",USART_Deal(&USART3_DataBuff,1));
        OLED_Printf(0,16,OLED_8X16,"V831:%d",USART_Deal(&UART5_DataBuff,1));
        OLED_Printf(0,32,OLED_8X16,"HFY:%d",USART_Deal(&UART4_DataBuff,1));
        OLED_Printf(0,48,OLED_8X16,"MPU6050:%f",yaw);
        OLED_Update();
    }
}


void Task4_sysControl()
{
    while(1)
    {
        Project_LIB_ControlTask();
    }
}