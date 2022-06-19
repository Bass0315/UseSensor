#include "User_OLED.h"

#if OLED64x48
extern Adafruit_SSD1306 display;
#endif

void OLED_Init(void)
{
  Wire.begin();
  #if (OLED96x96)
  {
    SeeedGrayOled.init(SH1107G);             //initialize SEEED OLED display
    SeeedGrayOled.setNormalDisplay();       //Set Normal Display Mode
    SeeedGrayOled.setVerticalMode();        // Set to vertical mode for displaying text
    SeeedGrayOled.clearDisplay();           //Clear Display.
  }

  #elif (OLED64x48)
  {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
  }
  #endif
}
void OLED_Write(uint8_t x, uint8_t y, char *string)
{
  #if (OLED96x96)
  {
    SeeedGrayOled.setTextXY(y, x);        //set Cursor to ith line, 0th column
    SeeedGrayOled.setGrayLevel(y);        //Set Grayscale level. Any number between 0 - 15.
    SeeedGrayOled.putString(string);      //Print Hello World
  }
  #elif(OLED64x48)
  {
    display.setCursor(32+x, 16+(6*y)); // 设置开始显示文字的坐标
    display.println(string); // 输出的字符
    display.display(); // 使更改的显示生效
  }
  #endif
}
