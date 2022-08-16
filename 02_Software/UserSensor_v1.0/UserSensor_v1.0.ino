//TCA9548A IIC HUB.
#include "User_TCA9548A_IICHub.h"
TCA9548A<TwoWire> TCA;
extern uint8_t TCA_CHANNEL[];

//OLED96x96.
#include "User_OLED.h"
#if OLED64x48
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
#endif

//Push Check.
#include "User_CheckPush.h"

//SHT4X
#include "User_SHT4X.h"
SHTSensor SHT4X(SHTSensor::SHT4X);

// PCF8574
#include "User_PCF8574.h"
PCF8574 PCF_8574(0x20);

// User_SerialSunriseCO2
#include "User_SerialSunriseCO2.h"
extern int readPeriodMs;
extern uint16_t state[];

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
          sprintf(strBuffer, "%d:OK", iChannel);
          Serial.println(strBuffer);
#if (OLED96x96)
          OLED_Write(0, iChannel, strBuffer);
#elif (OLED64x48)
          if (!(iChannel % 2))
            OLED_Write(0, iChannel, strBuffer);
          else
            OLED_Write(32, iChannel - 1, strBuffer);
#endif
        }
        TCA.closeAll();
      }
    }
    else
    {
      //Do something
      sprintf(strBuffer, "%d:ER", iChannel);
      Serial.println(strBuffer);
#if (OLED96x96)
      OLED_Write(0, iChannel, strBuffer);
#elif (OLED64x48)
      if (!(iChannel % 2))
        OLED_Write(0, iChannel, strBuffer);
      else
        OLED_Write(32, iChannel - 1, strBuffer);
#endif
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  TCA9548A_Init();
  OLED_Init();
  BH1721_Init();
  SHT4X_Init();
  Push_Init();
  Init_PCF8574();
  Init_SerialSunriseCO2();
  //while (!Serial);
}

bool b_KeyPush = false;
void loop()
{
  while (!b_fnGetPush())
  {
    b_KeyPush = false;
    //SeeedGrayOled.clearDisplay();
    //display.clearDisplay();
    LED_clearDisplay();
  }

  //test
  if (b_fnGetPush() && !b_KeyPush)
  {
    //fnReadBH1721();
    fnReadSHT4X();
    read_serialco2();

    b_KeyPush = true;
  }
}
