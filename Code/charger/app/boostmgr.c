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