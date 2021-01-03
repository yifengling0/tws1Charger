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

#include "powermgr.h"
#include "atom.h"
#include "debug.h"
#include "console.h"
#include "stm8s_adc1.h"

#include "stm8s003adc.h"

#define ADCBAT_PORT    GPIOD 
#define ADCBAT_PIN     (GPIO_PIN_2)

#define ADCOUT_PORT    GPIOD 
#define ADCOUT_PIN     (GPIO_PIN_3)

#define BATTER_LOW_LIMIT (3050)
#define CHARGER_LOW_LIMIT (10)


static Stm8s003Adc _battery;
static Stm8s003Adc _charge;

static FD _batteryFD;
static FD _chargeFD;

uint16_t power_mgr_get_battery_vol()
{
    uint16_t data;

    read(_batteryFD, (char*)&data, sizeof(uint16_t));

    return (data);
}

uint16_t power_mgr_get_charger_amp()
{
    uint16_t data;

    read(_chargeFD, (char*)&data, sizeof(uint16_t));

    return (data);
}

int  power_mgr_is_battery_low()
{
  uint16_t vol = power_mgr_get_battery_vol();
 
  return (vol <= BATTER_LOW_LIMIT);
}

int power_mgr_is_charge_amp_low()
{
    uint16_t vol = power_mgr_get_charger_amp();
    return (vol <= CHARGER_LOW_LIMIT);
}

void power_mgr_init()
{
    //0.1754385964912281 是47k + 10k 电阻分压的比例，具体可以参考原理图
    Stm8s003AdcCreate(&_battery, "batt", ADC1_CHANNEL_3, (uint32_t)ADCBAT_PORT, (uint8_t)ADCBAT_PIN, 0.1754385964912281);
    Stm8s003AdcCreate(&_charge, "chge", ADC1_CHANNEL_4,  (uint32_t)ADCOUT_PORT, (uint8_t)ADCOUT_PIN, 1.0);

    _batteryFD = open("batt", 0, 0);
    _chargeFD = open("chge", 0, 0);
}


