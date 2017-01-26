#ifndef __VEC2_H__
#define __VEC2_H__

#include <arduino.h>
#include "Util.h"

template <class T>
class Vec2
{
public:
	T x, y;
  Vec2() :x(0), y(0) {}
  Vec2(T x, T y) : x(x), y(y) {}
  Vec2 operator+(Vec2& v) 
  {
    return vec2(x + v.x, y + v.y);
  }
  Vec2 operator*(T s) 
  {
    return Vec2(x * s, y * s);
  }
  Vec2& operator*=(T s) 
  {
    x *= s;
    y *= s;
    return *this;
  }
  Vec2& normalize() 
  {
    if (length() == 0) return *this;
    *this *= ((T)1 / length());
    return *this;
  }
  T length() const 
  {
    return sqrt(x * x + y * y);
  }
  static Vec2 lerp(Vec2 a, Vec2 b, T ratio)
  {
    return Vec2(Util::lerp(a.x, b.x, ratio), Util::lerp(a.y, b.y, ratio));
  }
};
typedef Vec2<float> Vec2f;
#endif
