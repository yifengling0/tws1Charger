#ifndef _STM8S003ADCH_
#define _STM8S003ADCH_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm8s_adc1.h"
#include "adc.h"

typedef struct stm8s003Adc {
    Adc super;
    struct{
        int Channel;
        double Scale;
    }Protected;
}Stm8s003Adc;

extern void Stm8s003AdcCreate(Stm8s003Adc* adc, const char *name, int channel,   uint32_t port, uint8_t Pin, double scale);

#ifdef __cplusplus
}
#endif

#endif  /* _STM8S003ADCH_ */