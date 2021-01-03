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