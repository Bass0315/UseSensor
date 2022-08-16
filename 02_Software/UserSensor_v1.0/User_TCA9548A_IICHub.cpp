#include "User_TCA9548A_IICHub.h"

extern TCA9548A<TwoWire> TCA;

uint8_t TCA_CHANNEL[] = {TCA_CHANNEL_0, TCA_CHANNEL_1, TCA_CHANNEL_2, TCA_CHANNEL_3, TCA_CHANNEL_4, TCA_CHANNEL_5, TCA_CHANNEL_6, TCA_CHANNEL_7};

void TCA9548A_Init()
{
  TCA.begin(WIRE);
}


void ScanAddress(void)
{
  uint8_t error, i2cAddress, devCount, unCount;
  for (uint8_t i = 0; i < 8; i++)
  {
    TCA.openChannel(TCA_CHANNEL[i]);
    Serial.print("CHANNEL: ");
    Serial.println(i);

    devCount = 0;
    unCount = 0;
    for (i2cAddress = 1; i2cAddress < 127; i2cAddress++ )
    {
      WIRE.beginTransmission(i2cAddress);
      error = WIRE.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at 0x");
        if (i2cAddress < 16) Serial.print("0");
        Serial.println(i2cAddress, HEX);
        devCount++;
      }
      else if (error == 4)
      {
        Serial.print("Unknow error at 0x");
        if (i2cAddress < 16) Serial.print("0");
        Serial.println(i2cAddress, HEX);
        unCount++;
      }
    }

    if (devCount + unCount == 0)
      Serial.println("No I2C devices found\n");
    else {
      Serial.print(devCount);
      Serial.print(" device(s) found");
      if (unCount > 0) {
        Serial.print(", and unknown error in ");
        Serial.print(unCount);
        Serial.print(" address");
      }
      Serial.println();
    }
    Serial.println();
    delay(1000);
    TCA.closeChannel(TCA_CHANNEL[i]);
  }
}

bool findAddress(uint8_t chanel, uint8_t address)
{
  uint8_t error;
  
  TCA.openChannel(chanel);

  WIRE.beginTransmission(address);
  error = WIRE.endTransmission();
  if (error == 0)
  {
    Serial.print("I2CAddress 0x");
    if (address < 16)Serial.print("0");
    Serial.println(address, HEX);
    TCA.closeChannel(chanel);
    return true;
  }
  else if (error == 4)
  {
    Serial.print("Unknow 0x");
    if (address < 16) Serial.print("0");
    Serial.println(address, HEX);
  }
  TCA.closeChannel(chanel);
  return false;
}
