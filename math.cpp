#include "math.hpp"
#include <cmath>
#include <limits>

bool approx(double a, double b) {
    return a - b < std::numeric_limits<double>::epsilon();
}

Vec3D::Vec3D() {
    x = y = z = 0;
}

Vec3D::Vec3D(double x, double y, double z) : x(x), y(y), z(z) {};

const Vec3D Vec3D::operator+(const Vec3D &v) const {
    return Vec3D(x + v.x, y + v.y, z + v.z);
}

const Vec3D Vec3D::operator-(const Vec3D &v) const {
    return *this + -1.0 * v;
}

const Vec3D Vec3D::operator*(double d) const {
    return Vec3D(d * x, d * y, d * z);
}

double Vec3D::operator*(const Vec3D &v) const {
    return x * v.x + y * v.y + z * v.z;
}

double Vec3D::mag() const {
    return sqrt(*this * *this);
}

double Vec3D::mag2() const {
    return *this * *this;
}

const Vec3D Vec3D::norm() const {
    return 1/(this->mag()) * *this;
}

bool Vec3D::operator==(const Vec3D &v) const {
    return approx(x, v.x) && approx(y, v.y) && approx(z, v.z);
}

const Vec3D operator*(double d, const Vec3D &v) {
    return v * d;
}

const Vec3D crossProduct(const Vec3D &u, const Vec3D &v) {
    return Vec3D(u.y * v.z - u.z * v.y,
                 u.z * v.x - u.x * v.z,
                 u.x * v.y - u.y * v.z);
}

Ray::Ray(const Vec3D &from, const Vec3D &towards) : start(from),
                                        dir((towards - from).norm()) {}

Vec3D Ray::pointAt(double d) {
    return start + d * dir;
}

// Gives `vec' rotated around `around' by `theta' radians. See
// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
Vec3D rotate(const Vec3D &vec, const Vec3D &around, double theta) {
    Vec3D aroundNorm = around.norm();
    return Vec3D(// X
                 (cos(theta) + aroundNorm.x * aroundNorm.x *
                  (1 - cos(theta))) * vec.x +
                 (aroundNorm.x * aroundNorm.y * (1 - cos(theta)) -
                  aroundNorm.z * sin(theta)) * vec.y +
                 (aroundNorm.x * aroundNorm.z * (1 - cos(theta)) +
                  aroundNorm.y * sin(theta)) * vec.z,
                 // Y
                 (aroundNorm.y * aroundNorm.x * (1 - cos(theta)) +
                  aroundNorm.z * sin(theta)) * vec.x + 
                 (cos(theta) + aroundNorm.y * aroundNorm.y *
                  (1 - cos(theta))) * vec.y +
                 (aroundNorm.y * aroundNorm.z * (1 - cos(theta)) -
                  aroundNorm.x * sin(theta)) * vec.z,
                 // Z
                 (aroundNorm.z * aroundNorm.x * (1 - cos(theta)) -
                  aroundNorm.y * sin(theta)) * vec.x +
                 (aroundNorm.z * aroundNorm.y * (1 - cos(theta)) +
                  aroundNorm.x * sin(theta)) * vec.y +
                 (cos(theta) + aroundNorm.z * aroundNorm.z *
                  (1 - cos(theta))) * vec.z);
}
