#include "Util.h"

float Util::lerp(float a, float b, float x)
{
  return a + x * (b - a);
}

float Util::clamp(float x, float a, float b)
{
  return min(b, max(a, x));
}

float Util::clamp01(float x)
{
  return min(1.0f, max(.0f, x));
}

float Util::normalizeAngle(float angle)
{
  float rv = fmod(angle, M_PI * 2.0f);
  if (rv > M_PI) rv -= 2.0f * M_PI;
  if (rv < -M_PI) rv += 2.0f * M_PI;
  return rv;
}

float Util::lerpAngle(float a, float b, float x)
{
  float cs = (1.0f - x) * cos(a) + x * cos(b);
  float sn = (1.0f - x) * sin(a) + x * sin(b);
  return atan2(sn, cs);
}
