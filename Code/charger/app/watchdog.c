#include "watchdog.h"
#include "stm8s_iwdg.h"

#define  IWDG_500MS_REST  250
#define  IWDG_400MS_REST  200
#define  IWDG_300MS_REST  150
#define  IWDG_250MS_REST  125
#define  IWDG_200MS_REST  100
#define  IWDG_100MS_REST  50
#define  IWDG_50MS_REST   25
#define  IWDG_4MS_REST    2


void watchdog_init()
{
    IWDG_Enable();//启动独立看门狗

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//同意操作独立看门狗
    IWDG_SetPrescaler(IWDG_Prescaler_128);//看门狗预分频
    IWDG_SetReload(IWDG_500MS_REST);//设置看门狗复位时间
}

void watchdog_feed()
{
    IWDG_ReloadCounter(); //看门狗复位 正常使用时不想发生看门狗复位.
}