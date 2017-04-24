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

// Gives `vec' rotated around `around' by `theta' radians. See
// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
Vec3D rotate(Vec3D vec, Vec3D around, double theta) {
    around = around.norm();
    return Vec3D(// X
                 (cos(theta) + around.x * around.x * (1 - cos(theta))) * vec.x +
                 (around.x * around.y * (1 - cos(theta)) -
                  around.z * sin(theta)) * vec.y +
                 (around.x * around.z * (1 - cos(theta)) +
                  around.y * sin(theta)) * vec.z,
                 // Y
                 (around.y * around.x * (1 - cos(theta)) +
                  around.z * sin(theta)) * vec.x + 
                 (cos(theta) + around.y * around.y * (1 - cos(theta))) * vec.y +
                 (around.y * around.z * (1 - cos(theta)) -
                  around.x * sin(theta)) * vec.z,
                 // Z
                 (around.z * around.x * (1 - cos(theta)) -
                  around.y * sin(theta)) * vec.x +
                 (around.z * around.y * (1 - cos(theta)) +
                  around.x * sin(theta)) * vec.y +
                 (cos(theta) + around.z * around.z * (1 - cos(theta))) * vec.z);
}
