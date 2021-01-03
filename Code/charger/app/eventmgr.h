#ifndef _EVENTMGRH_
#define _EVENTMGRH_

#include "event.h"

#define EVENT_TIMER         (0)
#define EVENT_HALLSWITCH    (1)

void event_mgr_init(void);

void event_mgr_wait(ATOM_EX_EVENT *event, int timeout);

void event_mgr_set(int type, uint8_t param);

#endif