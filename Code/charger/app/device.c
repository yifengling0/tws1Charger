/*
 * Copyright (c) 2020, yifengling0. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
    // 代码空间不够，没有启动watchdog
    //watchdog_init();
}