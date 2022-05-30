//TCA9548A IIC HUB.
#include "User_TCA9548A_IICHub.h"
TCA9548A<TwoWire> TCA;

//OLED128x128.
#include "User_OLED96x96.h"

//Push Check.
#include "User_CheckPush.h"

extern uint8_t TCA_CHANNEL[];

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

void BH1721_Init(float rg_aOKChannel[][11])
{
  bh1721.begin();
  for (uint8_t i = 0; i < 8; i++)
  {
    if (findAddress(TCA_CHANNEL[i], BH1721_ADDR))
    {
      TCA.openChannel(TCA_CHANNEL[i]);
      if (!bh1721.startMeasure(BH1721::measModeAR))
      {
        Serial.println("Channel:" + String(TCA_CHANNEL[i]) + " ,Cannot start measurements.");
      }
      rg_aOKChannel[i][0] = 1;
      TCA.closeChannel(TCA_CHANNEL[i]);
    }
    else
    {
      Serial.println("Channel:" + String(TCA_CHANNEL[i]) + " ,Not found address.");
      rg_aOKChannel[i][0] = 0;
    }
  }
}


void fnGetBH1721Value(float re_aBH1721Value[][11])
{
  char strBuffer[20];
  float lux;
  SeeedGrayOled.clearDisplay();           //Clear Display.
  for (uint8_t iChannel = 0; iChannel < 8; iChannel++)
  {
    if (re_aBH1721Value[iChannel][0] == 1) //BH1721 address is online in this channel.
    {
      TCA.openChannel(TCA_CHANNEL[iChannel]);
      if (bh1721.readLux(&lux))
      {

        Serial.println("Luminosity: " + String(lux) + " lux");
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

/*
  void fnGetBH1721ValueAvg(void)
  {
  float lux;
  for (uint8_t iCount = 1; iCount < 11; i++)
  {
    for (uint8_t iChannel = 0; iChannel < 8; i++)
    {
      if (BH1721[iChannel][0])   //BH1721 address is online in this channel.
      {
        if (bh1721.readLux(&lux))
        {
          BH1721[iChannel][iCount] = lux;
        }
      }
      eles
      {
        continue;
      }
    }
  }
  }
*/

void setup() {
  Serial.begin(115200);
  TCA9548A_Init();
  OLED128x128_Init();
  Push_Init();
  while (!Serial);
  ScanAddress();
  
}

void loop()
{
  while (!b_fnGetPush());
  if (b_fnGetPush())
  {
    BH1721_Init(BH1721);
    fnGetBH1721Value(BH1721);
  }
}
