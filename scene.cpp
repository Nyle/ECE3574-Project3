#include "scene.hpp"

#include "json_format_error.hpp"
#include <fstream>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <limits>
#include <cmath>

double getDouble(const QJsonValue json) {
    if (!json.isDouble()) {
        throw JsonFormatError("Expected number but got non-number");
    }
    return json.toDouble();
}

size_t getSizeT(const QJsonValue json) {
    double d = getDouble(json);
    if (std::floor(d) != d) {
        throw JsonFormatError("Expected unsigned integer but got double");
    }
    if (d < 0) {
        throw JsonFormatError("Expected unsigned integer but got negative");
    }
    return (size_t) d;
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

Color colorFromJson(QJsonObject json) {
    size_t r = getSizeT(json["r"]);
    size_t g = getSizeT(json["g"]);
    size_t b = getSizeT(json["b"]);
    if (r > 255 || g > 255 || b > 255) {
        throw JsonFormatError("Colors must not be greater than 255");
    }
    return Color(r, g, b);
}

SceneObject::SceneObject(QJsonObject json) :
    center(vec3DFromJson(json["center"])),
    color(colorFromJson(json["color"].toObject())),
    lambert(getDouble(json["lambert"])) {
    if (lambert < 0 || lambert > 1) {
        throw JsonFormatError("Can't have lambert outside of [0, 1]");
    }
}

inline SceneObject::~SceneObject() {}

Sphere::Sphere(QJsonObject json) : SceneObject(json),
                                   radius(getDouble(json["radius"])) {
    if (radius < 0) {
        throw JsonFormatError("Can't have sphere with negative radius");
    }
}

double Sphere::intersect(const Ray &r) {
    double a = r.dir.mag2();
    double b = -2 * (center - r.start) * r.dir;
    double c = (center - r.start).mag2() - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) { return std::numeric_limits<double>::infinity(); }

    double d1 = (-b + sqrt(discriminant)) / (2 * a);
    double d2 = (-b - sqrt(discriminant)) / (2 * a);
    return fmin(d1 > 0 ? d1 : std::numeric_limits<double>::infinity(),
                d2 > 0 ? d2 : std::numeric_limits<double>::infinity());
}

Vec3D Sphere::normalAt(const Vec3D &point) {
    return (point - center).norm();
}

Plane::Plane(QJsonObject json) :
    SceneObject(json),
    normal(vec3DFromJson(json["normal"])) {}

Vec3D Plane::normalAt(const Vec3D &point) {
    return normal;
}

double Plane::intersect(const Ray &r) {
    // Ray and plane are parallel; no intersection
    if (r.dir * normal == 0) { return std::numeric_limits<double>::infinity(); }
    double res = ((center - r.start) * normal) / (r.dir * normal);
    return res > 0 ? res : std::numeric_limits<double>::infinity();
}


Light::Light(QJsonObject json) :
    location(vec3DFromJson(json["location"])),
    intensity(getDouble(json["intensity"])) {
    if (intensity < 0) {
        throw JsonFormatError("Can't have negative intensity");
    }
}

const Vec3D defaultCameraNormal = Vec3D(0, 0, 1);
const Vec3D defaultCameraVert = Vec3D(0, 1, 0);
const Vec3D defaultCameraHoriz = Vec3D(1, 0, 0);

Camera::Camera(QJsonObject json) :
    center(vec3DFromJson(json["center"])),
    normal(vec3DFromJson(json["normal"])),
    focus(getDouble(json["focus"])),
    sizex(getSizeT(json["size"].toArray()[0])),
    sizey(getSizeT(json["size"].toArray()[1])),
    resolutionx(getDouble(json["resolution"].toArray()[0])),
    resolutiony(getDouble(json["resolution"].toArray()[1])),
    focusPoint(center - normal.norm() * focus) {
    if (resolutionx < 0 || resolutiony < 0) {
        throw JsonFormatError("Can't have negative resolution");
    }
    yDir = defaultCameraVert;
    xDir = defaultCameraHoriz;
    Vec3D axis = crossProduct(defaultCameraNormal, normal);
    if (axis.mag2() != 0) {
        double angle = acos(defaultCameraNormal.norm() * normal.norm());
        yDir = rotate(defaultCameraVert, axis, angle);
        xDir = rotate(defaultCameraHoriz, axis, angle);
    }
    
}

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
            SceneObject * ptr = new Sphere(obj->toObject());
            objects.push_back(ptr);
        } else if (obj->toObject()["type"] == "plane") {
            SceneObject * ptr = new Plane(obj->toObject());
            objects.push_back(ptr);
        } else {
            for (auto obj : objects) delete obj;
            throw JsonFormatError("Objects must be of type sphere or plane");
        }
    }

    // Parse lights
    if (!json["lights"].isArray()) {
        for (auto obj : objects) delete obj;
        throw JsonFormatError("Expected 'lights' key to hold array");
    }
    QJsonArray jlights = json["lights"].toArray();
    for (auto obj = jlights.begin(); obj != jlights.end(); obj++) {
        if (!obj->isObject()) {
            for (auto obj : objects) delete obj;
            throw JsonFormatError("Malformed light");
        }
        lights.push_back(Light(obj->toObject()));
    }    
}

Scene::~Scene() {
    for (auto&&  obj : objects) {
        delete obj;
    }
}

std::string readFile(std::istream & stream) {
    std::string result{ std::istreambuf_iterator<char>(stream),
                        std::istreambuf_iterator<char>() };
    return result;
}


QJsonDocument strToJsonDocument(std::string str) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(str),
                                                &error);
    return doc;
}

Scene * loadSceneFromFile(std::string filename) {
    std::ifstream file(filename);
    QJsonDocument doc = strToJsonDocument(readFile(file));
    if (!doc.isObject()) {
        throw JsonFormatError("Expected top level to be Object");
    }
    return new Scene(doc.object());
}
