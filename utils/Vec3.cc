#include "Vec3.h"
#include <math.h>

double Vec3::Dot(const Vec3& lhs, const Vec3& rhs)
{
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 Vec3::Cross(const Vec3& lhs, const Vec3& rhs)
{
  double new_x = lhs.y * rhs.z - lhs.z * rhs.y;
  double new_y = lhs.z * rhs.x - lhs.x * rhs.z;
  double new_z = lhs.x * rhs.y - lhs.y * rhs.x;
  
  return Vec3(new_x, new_y, new_z);
}

Vec3::Vec3() : x(0), y(0), z(0), length(-1)
{
}

Vec3::Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z), length(-1)
{
}

Vec3::Vec3(const Vec3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), length(rhs.length)
{
}

Vec3 Vec3::Unit()
{
  return *this / this->Length();
}

Vec3& Vec3::operator=(const Vec3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    length = rhs.length;
    return *this;
}

Vec3& Vec3::operator+= (const Vec3& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  // Need to recalculate length
  length = -1;
  return *this;
}

Vec3& Vec3::operator/= (const double rhs)
{
  x /= rhs;
  y /= rhs;
  z /= rhs;
  length /= rhs;
  return *this;
}

Vec3 Vec3::operator/(const double rhs) const
{
  return Vec3(x / rhs, y / rhs, z / rhs);
}

Vec3 Vec3::operator*(const double rhs) const
{
  return Vec3(x * rhs, y * rhs, z * rhs);
}

// [TODO] elementwise multiplication, need to clarify this
Vec3 Vec3::operator*(const Vec3& rhs) const
{
  return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vec3 Vec3::operator-(const Vec3& rhs) const
{
  return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3 Vec3::operator+(const Vec3& rhs) const
{
  return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

bool Vec3::operator==(const Vec3& rhs)
{
  return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
}

double Vec3::Length()
{
  if (length == -1) {
    length = KNorm(2);
  }
  return length;
}

double Vec3::KNorm(int k)
{
  if (k <= 0) {
    return -1;
  }
  return pow(pow(abs(x), k) + pow(abs(y), k) + pow(abs(z), k), 1.0 / k);
}

std::ostream& operator<< (std::ostream& out, const Vec3& obj)
{
    out << "(" << obj.x << "," << obj.y << "," << obj.z << ")";
    return out;
}