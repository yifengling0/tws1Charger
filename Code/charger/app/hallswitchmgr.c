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

#include "hallswitchmgr.h"
#include "atom.h"
#include "atomsem.h"
#include "stm8s_gpio.h"
#include "stm8s_exti.h"
#include "eventmgr.h"

//霍尔开关外部中断引脚
#define HALL_SWITH_PORT        GPIOC 
#define HALL_SWITH_PIN         (GPIO_PIN_3)
#define HALL_SWITH_READ()	   (HALL_SWITH_PORT->IDR & HALL_SWITH_PIN)

//static ATOM_SEM sem1;

void hallswitch_mgr_init()
{
    //霍尔开关外部中断处理
    GPIO_Init(HALL_SWITH_PORT, HALL_SWITH_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL); //中断使能;
}

int hallswitch_mgr_get_status()
{
  return (HALL_SWITH_READ());
}

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector = 7
#endif
INTERRUPT void EXTI_PORTC_IRQHandler (void)
#if defined(__RCSTM8__)
interrupt 5

#elif defined(__SDCC_stm8)
__interrupt(5)
#endif
{
    /* Call the interrupt entry routine */
    atomIntEnter();

    //通知任务霍尔开关中断已经产生。
    event_mgr_set(EVENT_HALLSWITCH, hallswitch_mgr_get_status());

    /* Call the interrupt exit routine */
    atomIntExit(TRUE);
}