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
#include "stm8s.h"
#include "stm8s003adc.h"
#include "stm8s_adc1.h"

#include "stm8s_gpio.h"
#include "stm8s_adc1.h"
#include <string.h>

#define ADC_AVER_TIMES (10)
#define ADC_REF_VOL (1.22)

static uint16_t read_vol(ADC1_Channel_TypeDef channel, uint8_t isDelay)
{
    uint16_t result = 0;

    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, channel, ADC1_ALIGN_RIGHT); 
    ADC1_Cmd(ENABLE);

    ADC1_StartConversion();//启动ADC转换
    
    if( isDelay ){
        //开始转换稳定100ms
        atomTimerDelay(10);
    }

    for(int i = 0; i < ADC_AVER_TIMES; i++){
        while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);//等待转换完成
        ADC1_ClearFlag(ADC1_FLAG_EOC);//清除标准
        result += ADC1_GetConversionValue();//读取转换结果
    }

    ADC1_Cmd(DISABLE);

    return (result / ADC_AVER_TIMES);
}

static int adc_open(FD fd, int flags, int mode)
{
	return (fd);
}

static int adc_close(FD fd)
{
	return (OK);
}

static int adc_read(FD fd, char* buffer, size_t maxbytes)
{
    Stm8s003Adc* adc = (Stm8s003Adc*)fd;

    uint16_t ref = read_vol(ADC1_CHANNEL_7, 0);

    uint16_t result = read_vol((ADC1_Channel_TypeDef)adc->Protected.Channel, 1);

    double ret = (ADC_REF_VOL * (double)result) / ref / adc->Protected.Scale;

    uint16_t data = (uint16_t)(ret * 1000);

    memcpy(buffer, &data, sizeof(uint16_t));

    return (OK);
}

static int adc_ioctl(FD fd, int function, void* arg)
{
	return (NG);
}


void Stm8s003AdcCreate(Stm8s003Adc* adc, const char *name, int channel, uint32_t port, uint8_t pin, double scale)
{
    adc->Protected.Scale = scale;
    adc->Protected.Channel = channel;
    AdcCreate(&adc->super, name, adc_open, adc_read, adc_close, adc_ioctl);

    GPIO_Init((GPIO_TypeDef *) port, (GPIO_Pin_TypeDef)pin, GPIO_MODE_IN_FL_NO_IT);
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, (ADC1_Channel_TypeDef)channel, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, (ADC1_SchmittTrigg_TypeDef)channel, DISABLE);
}

