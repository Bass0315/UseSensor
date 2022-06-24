#include "User_PCF8574.h"

extern PCF8574 PCF_8574;

void Init_PCF8574(void)
{
  PCF_8574.begin();

  for (uint8_t i = 0; i < 8; i++)
  {
    doLow(i);
  }
}

void doHigh(uint8_t Pin)
{
  PCF_8574.write(Pin, HIGH);
  int x = PCF_8574.read8();
  //Serial.print("Read ");
  //Serial.println(x, HEX);
}


void doLow(uint8_t Pin)
{
  PCF_8574.write(Pin, LOW);
  int x = PCF_8574.read8();
  //Serial.print("Read ");
  //Serial.println(x, HEX);
}


void doToggle(uint8_t Pin)
{
  PCF_8574.toggle(Pin);
  int x = PCF_8574.read8();
  //Serial.print("Read ");
  //Serial.println(x, HEX);
}
