#include "Vec3.h"
#include <math.h>
#include <iostream>

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

Vec3::Vec3()
  : x(inList[0]), y(inList[1]), z(inList[2]), length(-1)
{
  inList[0] = 0;
  inList[1] = 0;
  inList[2] = 0;
}

Vec3::Vec3(double _x, double _y, double _z)
  : Vec3()
{
  inList[0] = _x;
  inList[1] = _y;
  inList[2] = _z;
}

Vec3::Vec3(const Vec3& rhs)
  : Vec3()
{
  length = rhs.length;
  inList[0] = rhs.inList[0];
  inList[1] = rhs.inList[1];
  inList[2] = rhs.inList[2];
}

Vec3 Vec3::Unit()
{
  return *this / this->Length();
}

Vec3& Vec3::operator=(const Vec3& rhs)
{
  length = rhs.length;
  inList[0] = rhs.inList[0];
  inList[1] = rhs.inList[1];
  inList[2] = rhs.inList[2];
  return *this;
}

Vec3& Vec3::operator+= (const Vec3& rhs)
{
  inList[0] += rhs.inList[0];
  inList[1] += rhs.inList[1];
  inList[2] += rhs.inList[2];
  // Need to recalculate length
  length = -1;
  return *this;
}

Vec3& Vec3::operator+= (const double rhs)
{
  inList[0] += rhs;
  inList[1] += rhs;
  inList[2] += rhs;
  // Need to recalculate length
  length = -1;
  return *this;
}

Vec3& Vec3::operator/= (const double rhs)
{
  inList[0] /= rhs;
  inList[1] /= rhs;
  inList[2] /= rhs;
  if (length != -1) {
    length /= abs(rhs);
  }
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

double& Vec3::operator[](const int index)
{
  return inList[index];
}

const double Vec3::operator[](const int index) const
{
  return inList[index];
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