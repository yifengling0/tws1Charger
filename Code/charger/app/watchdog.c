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

#include "watchdog.h"
#include "stm8s_iwdg.h"

#define  IWDG_500MS_REST  250
#define  IWDG_400MS_REST  200
#define  IWDG_300MS_REST  150
#define  IWDG_250MS_REST  125
#define  IWDG_200MS_REST  100
#define  IWDG_100MS_REST  50
#define  IWDG_50MS_REST   25
#define  IWDG_4MS_REST    2


void watchdog_init()
{
    IWDG_Enable();//启动独立看门狗

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//同意操作独立看门狗
    IWDG_SetPrescaler(IWDG_Prescaler_128);//看门狗预分频
    IWDG_SetReload(IWDG_500MS_REST);//设置看门狗复位时间
}

void watchdog_feed()
{
    IWDG_ReloadCounter(); //看门狗复位 正常使用时不想发生看门狗复位.
}