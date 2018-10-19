#pragma once

#include <ostream>


class Vec3 {
public:
  static double Dot(const Vec3& lhs, const Vec3& rhs);
  static Vec3 Cross(const Vec3& lhs, const Vec3& rhs);

  Vec3();
  Vec3(double _x, double _y, double _z);
  Vec3(const Vec3& rhs);
  Vec3& operator= (const Vec3& rhs);
  Vec3& operator+= (const Vec3& rhs);
  Vec3& operator+= (const double rhs);
  Vec3& operator/= (const double rhs);
  Vec3 operator/ (const double rhs) const;
  Vec3 operator* (const double rhs) const;
  Vec3 operator* (const Vec3& rhs) const;
  Vec3 operator- (const Vec3& rhs) const;
  Vec3 operator+ (const Vec3& rhs) const;
  bool operator== (const Vec3& rhs);
  double& operator[](const int index);
  const double operator[](const int index) const;

  friend std::ostream& operator<< (std::ostream& out, const Vec3& obj); 

  double Length();
  double KNorm(int k);
  Vec3 Unit();

  double& x;
  double& y;
  double& z;

protected:
  double inList[3];

private:
  double length;
};