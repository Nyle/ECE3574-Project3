#ifndef MATH_H
#define MATH_H

#include <cmath>

class Vec3D {
public:
    double x, y, z;
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

class Ray {
public:
    Vec3D start, dir;

    // Construct a Ray starting at `from' going through `towards'
    Ray(Vec3D from, Vec3D towards);
};

// const Vec3D intersection(Ray r, Sphere s);


#endif // MATH_H
