#include "atom.h"
#include "eventmgr.h"

#define EVENT_MAX (5)

static ATOM_EX_EVENT _eventBuffer[EVENT_MAX];

void event_mgr_init(void)
{
    atomExEventInit((uint8_t*)&_eventBuffer[0], sizeof(ATOM_EX_EVENT), EVENT_MAX);
}

void event_mgr_wait(ATOM_EX_EVENT *event, int timeout)
{
    atomExWaitEvent(event, timeout);
}

void event_mgr_set(int type, uint8_t param)
{
    ATOM_EX_EVENT e;
    e.type = type;
    e.param.data[0] = param;

    atomExSetEvent(&e);
}