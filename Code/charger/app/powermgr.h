#ifndef _POWERH_
#define _POWERH_

void power_mgr_init(void);
int power_mgr_is_battery_low(void);
int power_mgr_is_charge_amp_low(void);

#endif