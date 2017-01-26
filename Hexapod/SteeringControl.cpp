#include "SteeringControl.h"

void SteeringControl::setup()
{
  mySwitch.enableReceive(0);
  left = 0;
  right = 0;
}

void SteeringControl::update()
{
  if (mySwitch.available())
  {
    int value = mySwitch.getReceivedValue();

    if (value == 0)
    {
      Serial.print("Unknown encoding");
    }
    else
    {
      uint16_t packed = mySwitch.getReceivedValue();
      uint16_t x = packed & 0xFF;
      uint16_t y = packed >> 8;
      float xf = ((float)x / 255.0f) * 2.0f - 1.0f;
      float yf = ((float)y / 255.0f) * 2.0f - 1.0f;
      // dead zone
      xf = abs(xf) < .2f ? .0f : xf;
      yf = abs(yf) < .2f ? .0f : yf;
      left = yf + xf;
      right = yf - xf;
    }
    mySwitch.resetAvailable();
  }
}

float SteeringControl::getLeft()
{
  return left;
}

float SteeringControl::getRight()
{
  return right;
}
