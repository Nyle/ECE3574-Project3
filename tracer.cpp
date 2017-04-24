#include "tracer.hpp"

#include "scene.hpp"
#include <limits>

Vec3D focusPoint(Camera c) {
    return c.center - c.normal.norm() * c.focus;
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

const Vec3D defaultCameraNormal = Vec3D(0, 0, 1);
const Vec3D defaultCameraVert = Vec3D(0, 1, 0);
const Vec3D defaultCameraHoriz = Vec3D(1, 0, 0);

// Get the location of the pixel at coordinate (i, j)
const Vec3D pixelPoint(Camera c, size_t i, size_t j) {
    Vec3D rotatedVert = defaultCameraVert;
    Vec3D rotatedHoriz = defaultCameraHoriz;
    Vec3D axis = crossProduct(defaultCameraNormal, c.normal);
    if (axis.mag2() != 0) {
        double angle = acos(defaultCameraNormal.norm() * c.normal.norm());
        rotatedVert = rotate(defaultCameraVert, axis, angle);
        rotatedHoriz = rotate(defaultCameraHoriz, axis, angle);
    }

    return c.center + (i - 0.5 * c.sizex) * c.resolutionx * rotatedHoriz +
        (j - 0.5 * c.sizey) * c.resolutiony * rotatedVert;
}

Color trace(Scene * s, size_t i, size_t j) {
    Ray r = Ray(focusPoint(s->camera), pixelPoint(s->camera, i, j));
    double closestDistance = std::numeric_limits<double>::infinity();
    double tmp;
    SceneObject * closest;
    
    for (SceneObject * obj : s->objects) {
        tmp = obj->intersect(r);
        if (tmp < closestDistance) {
            closestDistance = tmp;
            closest = obj;
        }
    }

    if (closestDistance == std::numeric_limits<double>::infinity()) {
        return VOID_COLOR;
    }

    return closest->color;
}
