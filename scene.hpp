#ifndef SCENE_HPP
#define SCENE_HPP

class QJsonObject;
#include <vector>
#include "math.hpp"

class Color {
public:
    unsigned int r, g, b;

    // Constructor from json
    Color(QJsonObject json);

    // Constructor from values;
    Color(unsigned int r, unsigned int g, unsigned int b) : r(r), g(g), b(b) {}
};

// Interface for objects in the scene
class SceneObject {
public:
    Vec3D center;
    Color color;
    double lambert;

    SceneObject(QJsonObject json);
};

class Sphere : public SceneObject {
public:
    double radius;

    // Constructor from json
    Sphere(QJsonObject json);
};

class Plane : public SceneObject {
public:
    Vec3D normal;

    Plane(QJsonObject json);
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
    unsigned int sizex, sizey;
    double resolutionx, resolutiony;

    Camera(QJsonObject json);
};

class Scene {
public:
    std::vector<SceneObject> objects;
    std::vector<Light> lights;
    Camera camera;

    Scene(QJsonObject json);
};

#endif // SCENE_HPP
