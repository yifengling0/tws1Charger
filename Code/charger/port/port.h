#ifndef _PORTH_
#define _PORTH_

#include "atom.h"

#ifdef __cplusplus
extern "C" {
#endif

void PortInit(void);
void PortDebugPutChar(char c);
int  PortDebugIsReadable(void);
uint8_t PortDebugRead8(void);

int IsRamReadable(volatile void* address);
int IsRamReadWriteble(volatile void* address);

#ifdef __cplusplus
}
#endif

#endif  /* _PORTH_ */