//
#ifndef USER_PCF8574_H
#define USER_PCF8574_H

/*
 *
 */
#include "PCF8574.h"

void Init_PCF8574(void);
void doHigh(uint8_t Pin);
void doLow(uint8_t Pin);
void doToggle(uint8_t Pin);

#endif
