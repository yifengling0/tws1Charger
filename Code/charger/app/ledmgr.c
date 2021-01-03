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

#include "atom.h"
#include "ledmgr.h"
#include "pin.h"
#include "stm8s_gpio.h"

#define LED_PATTERN_SIZE (4)

#define LED_PORT  GPIOD 
#define LED_PIN   (GPIO_PIN_4)

static Pin _ledPin;

static int _index = 0;
static int _mode;

static const uint8_t _ledPattern[LED_MODE_MAX][LED_PATTERN_SIZE] ={
    { 1, 1, 1, 1 },
    { 0, 0, 1, 1 },
    { 0, 1, 0, 1 },
};

void led_mgr_init(void)
{
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
    PinCreate(&_ledPin, (void*)&LED_PORT->ODR, 8, (uint8_t)LED_PIN );
    PinWrite(&_ledPin, 1);
}

void led_mgr_set_mode(int mode)
{
    _mode = mode;
    _index = 0;
}

void led_mgr_tick(void)
{
    if( _mode != LED_MODE_NONE){
        PinWrite(&_ledPin, _ledPattern[_mode][_index]);

        _index++;
        if( _index == LED_PATTERN_SIZE){
            _mode = LED_MODE_NONE;
            _index = 0;
        }
    }
}