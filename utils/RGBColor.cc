#include "RGBColor.h"

RGBColor::RGBColor(double red, double green, double blue)
{
  x = (red < 0) ? 0 : ((red > 1 ? 1 : red));
  y = (green < 0) ? 0 : ((green > 1 ? 1 : green));
  z = (blue < 0) ? 0 : ((blue > 1 ? 1 : blue));
}

RGBColor::RGBColor(int red, int green, int blue)
{
  x = (red < 0) ? 0 : ((red > 255 ? 255 : red));
  y = (green < 0) ? 0 : ((green > 255 ? 255 : green));
  z = (blue < 0) ? 0 : ((blue > 255 ? 255 : blue));
}

RGBColor::RGBColor(const RGBColor& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
}

RGBColor::RGBColor(const Vec3& rhs)
{
  x = (rhs.x < 0) ? 0 : ((rhs.x > 1 ? 1 : rhs.x));
  y = (rhs.y < 0) ? 0 : ((rhs.y > 1 ? 1 : rhs.y));
  z = (rhs.z < 0) ? 0 : ((rhs.z > 1 ? 1 : rhs.z));
}

RGBColor RGBColor::To8Bit() const
{
  return RGBColor((int)(x * 255 + 0.5), (int)(y * 255 + 0.5), (int)(z * 255 + 0.5));
}

RGBColor RGBColor::MaxToOne() const
{
  double max_color = (x > y) ? (x > z ? x : z) : (y > z ? y : z);
  return max_color > 1 ? RGBColor(x / max_color, y / max_color, z / max_color) : *this;
}

RGBColor RGBColor::ClipToOne() const
{
  return RGBColor(x, y, z);
}