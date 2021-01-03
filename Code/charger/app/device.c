#include "device.h"
#include "uart.h"
#include "stm8s_clk.h"
#include "event.h"
#include "port.h"

/**
 * \b clk_init
 *
 * 初始化时钟
 *
 */
static void clk_init()
{
    CLK_HSICmd(ENABLE);//启用内部高速振荡器(HSI 16MHz)
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//主时钟=16MHZ/1=16MHz
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPU时钟=主时钟/1=18MHZ
}

/**
 * \b device_init
 *
 * 所有设备初始化
 *
 */
void device_init()
{
  
    clk_init();
    PortInit();
#ifndef USE_CONSOLE
    power_mgr_init();
    boost_mgr_init();
    hallswitch_mgr_init();
#endif    
    //watchdog_init();
}