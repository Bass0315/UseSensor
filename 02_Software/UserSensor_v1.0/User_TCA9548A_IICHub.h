#ifndef USER_TCA9548A_IICHUB_H
#define USER_TCA9548A_IICHUB_H

/*
 * https://files.seeedstudio.com/products/103020293/document/Grove_8Channel_I2C_Hub_test_library.zip
 */

#include "TCA9548A.h"
#include <Wire.h>
#define WIRE Wire


void TCA9548A_Init(void);
void ScanAddress(void);
bool findAddress(uint8_t chanel, uint8_t address);

#endif
