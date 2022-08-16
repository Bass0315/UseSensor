#ifndef USER_TH_H
#define USER_TH_H

/*
 * https://www.arduino.cc/reference/en/libraries/arduino-sht/
 */
#include <Wire.h>
#include "SHTSensor.h"
#include "User_TCA9548A_IICHub.h"
#include "User_OLED96x96.h"



extern TCA9548A<TwoWire> TCA;
extern uint8_t TCA_CHANNEL[];

void SHT4X_Init(void);
void fnReadSHT4X(void);

#endif
