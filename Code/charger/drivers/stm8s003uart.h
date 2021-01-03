#ifndef _STM8S003UARTH_
#define _STM8S003UARTH_

#include "sio.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct stm8s003Uart {
        SioBase super;
        struct {
            SIO_PARAM param;
        }Protected;
    }Stm8s003Uart;

   void Stm8s003UartCreate(Stm8s003Uart* sio, SIO_PARAM *param);
   void Stm8s003UartGetParam(Stm8s003Uart* sio, SIO_PARAM *param);
   
#ifdef __cplusplus
}
#endif

#endif  /* _STM8S003UARTH_ */