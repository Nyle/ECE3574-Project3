#ifndef MATH_H
#define MATH_H

#include <cmath>

class Sphere;
class Plane;

class Vec3D {
public:
    double x, y, z;
    // Default constructor
    Vec3D();
    // Constructor from values
    Vec3D(double x, double y, double z);
    // Vec3D addition
    const Vec3D operator+(const Vec3D v) const;
    // Vec3D subtraction
    const Vec3D operator-(const Vec3D v) const;
    // Scalar multiplication
    const Vec3D operator*(double d) const;
    // Dot product
    double operator*(const Vec3D v) const;
    // Magnitude
    double mag() const;
    // Square of the magnitude
    double mag2() const;
    // Normalized vector (magnitude of one, same direction)
    const Vec3D norm() const;
};

const Vec3D operator*(double d, const Vec3D v);

const Vec3D crossProduct(const Vec3D a, const Vec3D b);

class Ray {
public:
    Vec3D start, dir;

    // Construct a Ray starting at `from' going through `towards'
    Ray(Vec3D from, Vec3D towards);

    // Point along the ray that is `d' away from `start'
    Vec3D pointAt(double d);
};

Vec3D rotate(Vec3D vec, Vec3D around, double theta);

#endif // MATH_H
