#include <sdram.h>
#include <boot.h>
extern unsigned int _etext;
extern unsigned int _edata;
void Load_Code(void)
{
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *FlashAddr, *RamAddr;

    // Copy the code from flash to SDRAM
    LoadAddr = (unsigned int)&_etext;
    ExeAddr = SDRAM_START_ADDR;
    SectionLen = (unsigned int)&_edata - (unsigned int)&_etext;

    FlashAddr = (unsigned int *)LoadAddr;
    RamAddr = (unsigned int *)ExeAddr;

    for ( ; SectionLen != 0; SectionLen--)
    {
        *RamAddr++ = *FlashAddr++;
    }
}

void Boot_Init(void)
{
    // SDRAM初始化
    SDRAM_Init();
    // 加载代码
    Load_Code();
    // 设置栈顶地址
    // __set_MSP(*(uint32_t *)SDRAM_START_ADDR);
    // 设置重定位地址
    SCB->VTOR = SDRAM_START_ADDR;
    // set remap address
    SYSCFG->MEMRMP |= (1<<10);
    bsp_systick_delay_ms(1000);
    // 跳转到SDRAM
    ((void(*)(void))SDRAM_START_ADDR)();

}
