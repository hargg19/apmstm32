#ifndef ONOFF_TRIGGER_H
#define ONOFF_TRIGGER_H

#include "stm32f10x.h"

// Deklarasi fungsi
void OnOffTrigger_Init(void);
uint8_t OnOffTrigger_GetStatus1(void);
uint8_t OnOffTrigger_GetStatus2(void);

#endif // ONOFF_TRIGGER_H
