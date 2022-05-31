#include "User_OLED96x96.h"

void OLED96x96_Init(void)
{
  Wire.begin();
  SeeedGrayOled.init(SH1107G);             //initialize SEEED OLED display
  SeeedGrayOled.setNormalDisplay();       //Set Normal Display Mode
  SeeedGrayOled.setVerticalMode();        // Set to vertical mode for displaying text
  SeeedGrayOled.clearDisplay();           //Clear Display.
}
void OLED_Write(uint8_t x, uint8_t y, char *string)
{
  SeeedGrayOled.setTextXY(y, x);        //set Cursor to ith line, 0th column
  SeeedGrayOled.setGrayLevel(y);        //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(string);      //Print Hello World
}
