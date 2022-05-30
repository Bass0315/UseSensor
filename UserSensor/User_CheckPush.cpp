#include "User_CheckPush.h"


void Push_Init(void)
{
  pinMode(PushIO, INPUT);
}

bool b_fnGetPush(void)
{
  if (!digitalRead(PushIO))
  {
    delay(50);
    if (!digitalRead(PushIO))
    {
      //Serial.println("PUSH ON");
      return true;
    }
    else
    {
      //Serial.println("PUSH OFF");
      return false;
    }
  }
  else
  {
    //Serial.println("PUSH OFF");
    return false;
  }
}
