#ifndef MATH_H
#define MATH_H

#include <cmath>

// If the two numbers are approximately equal
bool approx(double a, double b);

class Vec3D {
public:
    double x, y, z;
    // Default constructor
    Vec3D();
    // Constructor from values
    Vec3D(double x, double y, double z);
    // Vec3D addition
    const Vec3D operator+(const Vec3D &v) const;
    // Vec3D subtraction
    const Vec3D operator-(const Vec3D &v) const;
    // Scalar multiplication
    const Vec3D operator*(double d) const;
    // Dot product
    double operator*(const Vec3D &v) const;
    // Magnitude
    double mag() const;
    // Square of the magnitude
    double mag2() const;
    // Normalized vector (magnitude of one, same direction)
    const Vec3D norm() const;
    // Equality operator. True if vectors have same three coordinates
    bool operator==(const Vec3D &v) const;
};

const Vec3D operator*(double d, const Vec3D &v);

const Vec3D crossProduct(const Vec3D &u, const Vec3D &v);

class Ray {
public:
    Vec3D start, dir;

    // Construct a Ray starting at `from' going through `towards'
    Ray(const Vec3D &from, const Vec3D &towards);

    // Point along the ray that is `d' away from `start'
    Vec3D pointAt(double d);
};

Vec3D rotate(const Vec3D &vec, const Vec3D &around, double theta);

#endif // MATH_H
