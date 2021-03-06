#ifndef SCENE_HPP
#define SCENE_HPP

class QJsonObject;
class QJsonDocument;
#include <vector>
#include <string>
#include "math.hpp"
#include "color.hpp"

// Interface for objects in the scene
class SceneObject {
public:
    Vec3D center;
    Color color;
    double lambert;

    SceneObject(QJsonObject json);
    // Returns the closest distance to the start of the ray where the ray
    // intersects the object they do not intersect.
    virtual double intersect(const Ray &r) = 0;

    // Returns a vertor normal to the surface of the object at the specified
    // point. The point should be on the surface of this object
    virtual Vec3D normalAt(const Vec3D &point) = 0;

    virtual ~SceneObject() = 0;
};

class Sphere : public SceneObject {
public:
    double radius;

    // Constructor from json
    Sphere(QJsonObject json);

    double intersect(const Ray &r);
    Vec3D normalAt(const Vec3D &point);
};

class Plane : public SceneObject {
public:
    Vec3D normal;

    Plane(QJsonObject json);

    double intersect(const Ray &r);
    Vec3D normalAt(const Vec3D &point);
};

class Light {
public:
    Vec3D location;
    double intensity;

    Light(QJsonObject json);

    Light(Vec3D location, double intensity) : location(location),
                                               intensity(intensity) {}
};

class Camera {
public:
    Vec3D center, normal;
    double focus;
    size_t sizex, sizey;
    double resolutionx, resolutiony;
    // Calculated at initialization
    Vec3D focusPoint, xDir, yDir;

    Camera(QJsonObject json);
};

class Scene {
public:
    std::vector<SceneObject *> objects;
    std::vector<Light> lights;
    Camera camera;

    Scene(QJsonObject json);
    ~Scene();
};

QJsonDocument strToJsonDocument(std::string str);
Scene * loadSceneFromFile(std::string filename);

#endif // SCENE_HPP
