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