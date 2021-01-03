#ifndef _LEDMGRH_
#define _LEDMGRH_

#define LED_MODE_NONE   (0)
#define LED_MODE_SWITCH (1)
#define LED_MODE_ERROR  (2)

#define LED_MODE_MAX    (3)
#define LED_TICK        (5)

void led_mgr_init(void);
void led_mgr_set_mode(int mode);
void led_mgr_tick(void);

#endif

