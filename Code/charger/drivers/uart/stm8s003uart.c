#include "iosdrv.h"

#include <string.h>

#include "stm8s003uart.h"
#include "atom.h"
#include "stm8s_uart1.h"

#define USE_SEND_BLOCKING (1) 
#define TMP_SIZE (10)

#define RX_BUF_SIZE (64)
#define TX_BUF_SIZE (8)

static uint8_t _rxBuffer[RX_BUF_SIZE];
static uint8_t _txBuffer[TX_BUF_SIZE];

static RINGBUFFER _rxRingBuffer;
static RINGBUFFER _txRingBuffer;

static int uart_close (SioBase* sio)
{
    UART1_ITConfig(UART1_IT_RXNE, DISABLE);
    UART1_ITConfig(UART1_IT_TXE, DISABLE);
    UART1_Cmd(DISABLE);
    UART1_DeInit();
    return (OK);
}

static void uart_do_send (SioBase* sio)
{
#if USE_SEND_BLOCKING
    char buffer[TMP_SIZE];
    int count = 0;

    CRITICAL_STORE;
    
    do{
        CRITICAL_START();
        count = ringGet(&_txRingBuffer, buffer, TMP_SIZE);
        CRITICAL_END();
        for(int i=0; i < count; i++){
            CRITICAL_START();
            UART1_SendData8(buffer[i]);
            /* Loop until the end of transmission */
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
            CRITICAL_END();
        }
    }while(count > 0);
    /* Return mutex access */

#else
     UART1_ITConfig(UART1_IT_TXE, ENABLE);
#endif
}

static int uart_ioctl(SioBase*sio, int function, void* arg)
{
    //TODO 波特率修改，等其他扩展功能
    return NG;
}

static UART1_StopBits_TypeDef get_stop_bit(enSioStopBit stopBit)
{
    if(stopBit == ssb2Bit){
        return UART1_STOPBITS_2;
    }else{
        return UART1_STOPBITS_1;
    }
}

static UART1_Parity_TypeDef get_parity(enSioParity parity)
{
    UART1_Parity_TypeDef ret = UART1_PARITY_NO;
    switch(parity){
        case spNone:
        ret = UART1_PARITY_NO;
        break;
        case spOdd:
        ret = UART1_PARITY_ODD;
        break;
        case spEven:
        ret = UART1_PARITY_EVEN;
        break;
        default:
        break;
    }

    return (ret);
}

static int uart_open(SioBase* sio, int flags, int mode)
{
    /**
     * Set up UART2 for putting out debug messages.
     * This the UART used on STM8S Discovery, change if required.
     */
    Stm8s003Uart *uart = (Stm8s003Uart*)sio;

    UART1_Init (uart->Protected.param.baud, 
                uart->Protected.param.dataBits == 8 ? UART1_WORDLENGTH_8D : UART1_WORDLENGTH_9D, 
                get_stop_bit(uart->Protected.param.stopBit), 
                get_parity(uart->Protected.param.parity),
                UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1_Cmd(ENABLE);

    return (TRUE);
}


void uart_rx_handler(void)
{
    CRITICAL_STORE;

    if(UART1_GetITStatus(UART1_IT_RXNE) == SET){
        UART1_ClearITPendingBit(UART1_IT_RXNE);
        CRITICAL_START();
        uint8_t c = UART1_ReceiveData8();
        ringPut(&_rxRingBuffer, (char*)&c, 1);
        CRITICAL_END();
    }
}

void uart_tx_handler(void)
{
   if(UART1_GetITStatus(UART1_IT_TXE) == SET){
        CRITICAL_STORE;
        int count = 0;
        char c;
        CRITICAL_START();
        count = ringGet(&_txRingBuffer, &c, 1);
        CRITICAL_END();
        if( count > 0){
            UART1_SendData8(c);
        }else{
            UART1_ITConfig(UART1_IT_TXE, DISABLE);
        }
    }
}



#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector = 20
#endif
INTERRUPT void UART1_RxISRHandler (void)
#if defined(__RCSTM8__)
interrupt 18

#elif defined(__SDCC_stm8)
__interrupt(18)
#endif
{
    /* Call the interrupt entry routine */
    atomIntEnter();

    uart_rx_handler();

    /* Call the interrupt exit routine */
    atomIntExit(TRUE);
}

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector = 19
#endif
INTERRUPT void UART1_TxISRHandler (void)
#if defined(__RCSTM8__)
interrupt 17

#elif defined(__SDCC_stm8)
__interrupt(17)
#endif
{
    /* Call the interrupt entry routine */
    atomIntEnter();

    uart_tx_handler();

    /* Call the interrupt exit routine */
    atomIntExit(TRUE);
}

void Stm8s003UartCreate(Stm8s003Uart* sio, SIO_PARAM *param)
{
    ringCreate(&_rxRingBuffer, _rxBuffer, RX_BUF_SIZE);
	ringCreate(&_txRingBuffer, _txBuffer, TX_BUF_SIZE);

    SioCreate(&sio->super, param->name, &_rxRingBuffer, &_txRingBuffer, uart_open, uart_do_send,
            uart_close, uart_ioctl);

    memcpy((void*)&sio->Protected.param, (void*)param, sizeof(SIO_PARAM));
    UART1_DeInit();
}

void Stm8s003UartGetParam(Stm8s003Uart* sio, SIO_PARAM *param)
{
    memcpy(param, &sio->Protected.param, sizeof(SIO_PARAM));
}