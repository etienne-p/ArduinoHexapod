#ifndef UTIL_H
#define UTIL_H

#include <arduino.h>
#include "Consts.h"

class Util
{
public:
	static float lerp(float a, float b, float x);
	static float clamp01(float x);
  static float clamp(float x, float a, float b);
  static float normalizeAngle(float angle);
  static float lerpAngle(float a, float b, float x);
};

#endif
