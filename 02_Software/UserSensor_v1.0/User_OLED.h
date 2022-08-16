//
#ifndef USER_OLED_H
#define USER_OLED_H

/*
     https://github.com/Seeed-Studio/OLED_Display_96X96
     https://wiki.seeedstudio.com/Grove-OLED-Display-0.66-SSD1306_v1.0/
*/

//OLED TYPE
#define OLED96x96  1
#define OLED64x48  0

#include <Wire.h>
#include <SeeedGrayOLED.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


void OLED_Init(void);
void OLED_Write(uint8_t x, uint8_t y, char *string);
void LED_clearDisplay(void);


#endif
