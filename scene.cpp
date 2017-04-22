#include "scene.hpp"

#include "json_format_error.hpp"
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>

double getDouble(const QJsonValue json) {
    if (!json.isDouble()) {
        throw JsonFormatError("Expected number but got non-number");
    }
    return json.toDouble();
}

unsigned int getUInt(const QJsonValue json) {
    double d = getDouble(json);
    if (std::floor(d) != d) {
        throw JsonFormatError("Expected unsigned integer but got double");
    }
    if (d < 0) {
        throw JsonFormatError("Expected unsigned integer but got negative");
    }
    return (unsigned int) d;
}

void checkIsObject(QJsonValue json) {
    if (!json.isObject()) {
        throw JsonFormatError("Expected object but got non-object");
    }
}

Vec3D vec3DFromJson(QJsonValue json) {
    checkIsObject(json);
    return Vec3D(getDouble(json.toObject()["x"]),
                  getDouble(json.toObject()["y"]),
                  getDouble(json.toObject()["z"]));
}

Color::Color(QJsonObject json) : r(getUInt(json["r"])),
                                 g(getUInt(json["g"])),
                                 b(getUInt(json["b"])) {}

SceneObject::SceneObject(QJsonObject json) :
    center(vec3DFromJson(json["center"])),
    color(Color(json["color"].toObject())),
    lambert(getDouble(json["lambert"])) {}

Sphere::Sphere(QJsonObject json) : SceneObject(json),
                                   radius(getDouble(json["radius"])) {}

Plane::Plane(QJsonObject json) :
    SceneObject(json),
    normal(vec3DFromJson(json["normal"])) {}

Light::Light(QJsonObject json) :
    location(vec3DFromJson(json["location"])),
    intensity(getDouble(json["intensity"])) {}

Camera::Camera(QJsonObject json) :
    center(vec3DFromJson(json["center"])),
    normal(vec3DFromJson(json["normal"])),
    focus(getDouble(json["focus"])),
    sizex(getUInt(json["size"].toArray()[0])),
    sizey(getUInt(json["size"].toArray()[1])),
    resolutionx(getDouble(json["resolution"].toArray()[0])),
    resolutiony(getDouble(json["resolution"].toArray()[1])) {}

Scene::Scene(QJsonObject json) : camera(Camera(json["camera"].toObject())) {
    // Parse objects
    if (!json["objects"].isArray()) {
        throw JsonFormatError("Expected 'objects' key to hold array");
    }
    QJsonArray jobjects = json["objects"].toArray();
    for (auto obj = jobjects.begin(); obj != jobjects.end(); obj++) {
        if (!obj->isObject()) {
            throw JsonFormatError("Malformed object");
        }
        if (obj->toObject()["type"] == "sphere") {
            objects.push_back(Sphere(obj->toObject()));
        } else if (obj->toObject()["type"] == "plane") {
            objects.push_back(Plane(obj->toObject()));
        } else {
            throw JsonFormatError("Objects must be of type sphere or plane");
        }
    }

    // Parse lights
    if (!json["lights"].isArray()) {
        throw JsonFormatError("Expected 'lights' key to hold array");
    }
    QJsonArray jlights = json["lights"].toArray();
    for (auto obj = jlights.begin(); obj != jlights.end(); obj++) {
        if (!obj->isObject()) {
            throw JsonFormatError("Malformed light");
        }
        lights.push_back(Light(obj->toObject()));
    }    
}
