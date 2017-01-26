#ifndef STEERING_CONTROL_H
#define STEERING_CONTROL_H

#include <RCSwitch.h>
#include "Vec2.h"
#include "Util.h"
#include "Consts.h"

class SteeringControl
{
public:
  void setup();
  void update();
  float getLeft(); 
  float getRight(); 
private:
  RCSwitch mySwitch;
  float left;
  float right;
};

#endif
