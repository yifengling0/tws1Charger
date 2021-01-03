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

#include "port.h"
#include "debug.h"
#include "stm8s003uart.h"
#include "stm8s003uart.h"

#ifdef USE_CONSOLE  

static SIO_PARAM _uart_param = {
    "uart",
    115200,
    8,
    ssb1Bit,
    spNone
};

static Stm8s003Uart _uart;

static FD _debugFD = 0;

void PortInit(void)
{
    Stm8s003UartCreate(&_uart, &_uart_param);
    _debugFD = open("uart", 0, 0);
}

void PortDebugPutChar(char c)
{
    write(_debugFD, &c, 1);
}

int  PortDebugIsReadable(void)
{
    uint8_t result;
       
    ioctl(_debugFD, SIO_RX_ISEMPTY, &result);

    return !result;
}
 
uint8_t PortDebugRead8(void)
{
    char c;
    read(_debugFD, &c, 1);

    return c;
}


int IsRamReadable(volatile void* address)
{
	return 1;
}

int IsRamReadWriteble(volatile void* address)
{
	return 1;
}

#else

void PortInit(void)
{
}

void PortDebugPutChar(char c)
{
}

int  PortDebugIsReadable(void)
{
    return 0;
}
 
uint8_t PortDebugRead8(void)
{
    return(0);      
}


int IsRamReadable(volatile void* address)
{
	return 1;
}

int IsRamReadWriteble(volatile void* address)
{
	return 1;
}
#endif