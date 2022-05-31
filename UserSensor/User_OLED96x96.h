//
#ifndef USER_OLED96X96_H
#define USER_OLED96X96_H

/*
 * https://github.com/Seeed-Studio/OLED_Display_96X96
 */

#include <Wire.h>
#include <SeeedGrayOLED.h>


void OLED96x96_Init(void);
void OLED_Write(uint8_t x,uint8_t y,char *string);


#endif
