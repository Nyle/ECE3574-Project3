#include "math.hpp"
#include <cmath>
#include "scene.hpp"

Vec3D::Vec3D(double x, double y, double z) : x(x), y(y), z(z) {};

const Vec3D Vec3D::operator+(const Vec3D v) const {
    return Vec3D(x + v.x, y + v.y, z + v.z);
}

const Vec3D Vec3D::operator-(const Vec3D v) const {
    return *this + -1.0 * v;
}

const Vec3D Vec3D::operator*(double d) const {
    return Vec3D(d * x, d * y, d * z);
}

double Vec3D::operator*(const Vec3D v) const {
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

const Vec3D operator*(double d, const Vec3D v) {
    return v * d;
}

const Vec3D crossProduct(const Vec3D u, const Vec3D v) {
    return Vec3D(u.y * v.z - u.z * v.y,
                 u.z * v.x - u.x * v.z,
                 u.x * v.y - u.y * v.z);
}

Ray::Ray(Vec3D from, Vec3D towards) : start(from),
                                      dir((towards - from).norm()) {}

Vec3D Ray::pointAt(double d) {
    return start + d * dir;
}
