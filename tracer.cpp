#include "tracer.hpp"

#include "scene.hpp"
#include <iostream>
#include <cmath>
#include <limits>

// Get the location of the pixel at coordinate (i, j)
const Vec3D pixelPoint(Camera c, size_t i, size_t j) {
    return c.center + (i - 0.5 * c.sizex) * c.resolutionx * c.xDir +
        (j - 0.5 * c.sizey) * c.resolutiony * c.yDir;
}

// Finds the object in `s' which intersects `r' closest to its start. `closest'
// will be the set to the found object, and `closestDistance' will be set to
// the distance from the start of `r' where the intersection happens.
void getIntersection(Scene * s, Ray r, SceneObject * & closest,
                     double & closestDistance) {
    closestDistance = std::numeric_limits<double>::infinity();
    double tmp;
    
    for (SceneObject * obj : s->objects) {
        tmp = obj->intersect(r);
        if (tmp < closestDistance) {
            closestDistance = tmp;
            closest = obj;
        }
    }
    return;
}

// Find if there is another object beside `exclude' between the start of the ray
// and `stopAt' along the ray
bool isShadowed(Scene * s, Ray r, SceneObject * exclude, double stopAt) {
    for (SceneObject * obj : s->objects) {
        if (obj == exclude) continue;
        if (obj->intersect(r) <= stopAt) return true;
    }
    return false;
}

Color trace(Scene * s, size_t i, size_t j) {
    Color result = VOID_COLOR;
    Ray r = Ray(s->camera.focusPoint, pixelPoint(s->camera, i, j));
    double closestDistance;
    SceneObject * closest;
    getIntersection(s, r, closest, closestDistance);

    if (closestDistance == std::numeric_limits<double>::infinity()) {
        return result;
    }

    Vec3D intersectionPt = r.pointAt(closestDistance);
    Vec3D intersectionNormal = closest->normalAt(intersectionPt);

    for (auto light : s->lights) {
        r = Ray(intersectionPt, light.location);
        double cameraPlaneIntersectionDist = r.dir * s->camera.normal == 0 ?
            std::numeric_limits<double>::infinity() :
            ((s->camera.center - r.start) * s->camera.normal) /
            (r.dir * s->camera.normal);
        
        if (!isShadowed(s, r, closest,
                             // We only care about ones that are between the
                             // light and the intersection point, and are also
                             // on the far side of the camera plane
                             fmin((intersectionPt - light.location).mag(),
                                  cameraPlaneIntersectionDist))) {
            double scale = intersectionNormal * r.dir * closest->lambert;
            if (scale >= 0) result += scale * light.intensity * closest->color;
        }
    }

    return result;
}
