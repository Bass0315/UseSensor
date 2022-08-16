#include "User_SHT4X.h"

extern SHTSensor sht4x(SHTSensor::SHT4X);

void SHT4X_Init(void)
{
  sht4x.init();
}

bool CO2TH_sta = true;
void fnReadSHT4X(void)
{
  char strBuffer[20];
  for (uint8_t iChannel = 0; iChannel < 8; iChannel++)
  {
    TCA.openChannel(TCA_CHANNEL[iChannel]);
    // read from second sensor
    if (sht4x.readSample())
    {
      sprintf(strBuffer, "Ch: %d, T:%0.2f, H:%0.2f", iChannel, sht4x.getTemperature(), sht4x.getHumidity());
      Serial.println(strBuffer);
      sprintf(strBuffer, "%d:OK", iChannel);


      OLED_Write(0, iChannel, strBuffer);

    }
    else
    {
      //Serial.print("Sensor 2: Error in readSample()\n");
      sprintf(strBuffer, "%d:ER", iChannel);
      Serial.println(strBuffer);

      OLED_Write(0, iChannel, strBuffer);
    }
    TCA.closeAll();
  }
}
