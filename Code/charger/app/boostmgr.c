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

#include "boostmgr.h"
#include "stm8s_gpio.h"
#include "atom.h"
#include "debug.h"
#include "console.h"

// 升压
#define BOOST_EN_PORT    GPIOC 
#define BOOST_EN_PIN     (GPIO_PIN_6)
#define BOOST_EN_HIGH() (BOOST_EN_PORT->ODR |= BOOST_EN_PIN)
#define BOOST_EN_LOW() (BOOST_EN_PORT->ODR &= ~BOOST_EN_PIN)

void boost_mgr_init()
{
    //初始化BoostEn升压输出
    GPIO_Init(BOOST_EN_PORT, BOOST_EN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
}

void boost_mgr_set_boost(int onoff)
{
    if( onoff == 1){
        BOOST_EN_HIGH();
    }else{
        BOOST_EN_LOW();
    }
}