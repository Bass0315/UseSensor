//TCA9548A IIC HUB.
#include "User_TCA9548A_IICHub.h"
TCA9548A<TwoWire> TCA;
extern uint8_t TCA_CHANNEL[];

//OLED128x128.
#include "User_OLED96x96.h"

//Push Check.
#include "User_CheckPush.h"

//SHT4X
#include "User_SHT4X.h"
SHTSensor SHT4X(SHTSensor::SHT4X);

//Serial CO2
#include "User_SerialCO2.h"

const int BH1721_ADDR = 0x23;

//Driver code for the BH1721
class BH1721 {
  public:

    enum measMode
    {
      measModeAR,
      measModeHR,
      measModeLR,
    };

    bool begin()
    {
      Wire.begin();
      return true;
    }

    bool setMeasTime(int mt) {
      if ((mt < MT_MIN) || (mt > MT_MAX))
      {
        return false;
      }
      return (sendCmd(CMD_MT_HIGH | (mt >> 5)) && sendCmd(CMD_MT_LOW | (mt & 0x10)));
    }

    bool startMeasure(enum measMode mode)
    {
      byte cmd;

      switch (mode)
      {
        case measModeAR:
          cmd = CMD_AR_MODE;
          break;
        case measModeHR:
          cmd = CMD_HR_MODE;
          break;
        case measModeLR:
          cmd = CMD_LR_MODE;
          break;
        default:
          return false;
      }
      return sendCmd(cmd);
    }

    bool readLux(float *lux)
    {
      if (Wire.requestFrom(BH1721_ADDR, 2) != 2)
      {
        return false;
      }
      byte msb = Wire.read();
      byte lsb = Wire.read();
      *lux = (msb * 0x100U | lsb) / 1.2;
      return true;
    }

  private:
    bool sendCmd(byte cmd)
    {
      Wire.beginTransmission(BH1721_ADDR);
      int size = Wire.write(cmd);
      if ((Wire.endTransmission() != 0) || (size != 1))
      {
        return false;
      }
      return true;
    }

    static const byte CMD_POWER_DOWN = 0x00;
    static const byte CMD_POWER_ON = 0x01;
    static const byte CMD_AR_MODE = 0x10;
    static const byte CMD_HR_MODE = 0x12;
    static const byte CMD_LR_MODE = 0x13;
    static const byte CMD_MT_HIGH = 0x40;
    static const byte CMD_MT_LOW = 0x60;

    static const int MT_MIN = 140;
    static const int MT_MAX = 1020;
};

BH1721 bh1721;

float BH1721[8][11] = {0};   //BH1721[?][0] is address flage.

void BH1721_Init(void)
{
  bh1721.begin();
}


void fnReadBH1721(void)
{
  char strBuffer[20];
  float lux;
  SeeedGrayOled.clearDisplay();           //Clear Display.
  for (uint8_t iChannel = 0; iChannel < 8; iChannel++)
  {
    if (findAddress(TCA_CHANNEL[iChannel], BH1721_ADDR))
    {
      TCA.openChannel(TCA_CHANNEL[iChannel]);
      if (!bh1721.startMeasure(BH1721::measModeAR))
      {
        Serial.println("Channel:" + String(TCA_CHANNEL[iChannel]) + " ,Cannot start measurements.");
        break;
      }
      if (bh1721.readLux(&lux))
      {
        //Serial.println("Luminosity: " + String(lux) + " lux");
        //Serial.println("Ch: " + String(iChannel) + ", " + String(lux) + " lux");
        sprintf(strBuffer, "Ch: %d,  %0.2f lux.", iChannel, lux);
        Serial.println(strBuffer);
        if (lux >= 0)
        {
          //Do something
          sprintf(strBuffer, "Ch: %d OK.", iChannel);
          Serial.println(strBuffer);
          OLED_Write(0, iChannel * 2, strBuffer);
        }
        TCA.closeChannel(TCA_CHANNEL[iChannel]);
      }
    }
    else
    {
      //Do something
      sprintf(strBuffer, "Ch: %d ER.", iChannel);
      Serial.println(strBuffer);
      OLED_Write(0, iChannel * 2, strBuffer);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  TCA9548A_Init();
  //OLED96x96_Init();
  BH1721_Init();
  //SHT4X_Init();
  Push_Init();
  while (!Serial);
}

void loop()
{
  while (!b_fnGetPush());
  if (b_fnGetPush())
  {
    fnReadBH1721();
    //fnReadSHT4X();
  }
}
