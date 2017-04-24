#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "scene.hpp"
#include "color.hpp"
#include "math.hpp"
#include "tracer.hpp"
#include "image_write.hpp"
#include "json_format_error.hpp"

#include <limits>
#include <sstream>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

std::ostream& operator << ( std::ostream& os, Vec3D const& v ) {
    os << "Vec3D(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

QJsonObject strToJsonObject(std::string str) {
    QJsonDocument doc = strToJsonDocument(str);
    REQUIRE(doc.isObject());
    return doc.object();
}

TEST_CASE("Test reading JSON inputs into sphere object", "[scene]") {
    REQUIRE_NOTHROW(Sphere(strToJsonObject("{\"center\": "
                                           "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                           "\"color\": "
                                           "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                           "\"lambert\": 1,"
                                           "\"radius\": 1,"
                                           "\"type\": \"sphere\""
                                           "}")));
}

TEST_CASE("Test reading JSON inputs into Plane object", "[scene]") {
    REQUIRE_NOTHROW(Plane(strToJsonObject("{\"center\": "
                                           "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                           "\"normal\": "
                                           "{ \"x\": 1, \"y\": 0, \"z\": 0},"
                                           "\"color\": "
                                           "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                           "\"lambert\": 1,"
                                           "\"type\": \"plane\""
                                           "}")));
}


TEST_CASE("Test reading JSON inputs into Camera object", "[scene]") {
    REQUIRE_NOTHROW(Camera(strToJsonObject("{\"center\": "
                                           "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                           "\"focus\": 10,"
                                           "\"normal\": "
                                           "{ \"x\": 0, \"y\": 1, \"z\": 0},"
                                           "\"resolution\": [0.01, 0.01],"
                                           "\"size\": [256,256]"
                                           "}")));
}

TEST_CASE("Test reading JSON inputs into Light object", "[scene]") {
    REQUIRE_NOTHROW(Light(strToJsonObject("{\"location\": "
                                          "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                           "\"intensity\": 3"
                                           "}")));
}


TEST_CASE("Test reading JSON inputs into a Scene object", "[scene]") {
    REQUIRE_NOTHROW(Scene(strToJsonObject("{\"camera\":"
                                          "{\"center\": "
                                          "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                          "\"focus\": 10,"
                                          "\"normal\": "
                                          "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                          "\"resolution\": [0.01, 0.01],"
                                          "\"size\": [256,256]},"
                                          "\"lights\":"
                                          "[{\"intensity\": 1,"
                                          "\"location\": "
                                          "{ \"x\": 5, \"y\": -5, \"z\": 0}}],"
                                          "\"objects\":"
                                          "[{\"center\": "
                                          "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                          "\"color\": "
                                          "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                          "\"lambert\": 1,"
                                          "\"radius\": 1,"
                                          "\"type\": \"sphere\"},"
                                          "{\"center\": "
                                          "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                          "\"normal\": "
                                          "{ \"x\": 1, \"y\": 0, \"z\": 0},"
                                          "\"color\": "
                                          "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                          "\"lambert\": 1,"
                                          "\"type\": \"plane\""
                                          "}]}")));
}

TEST_CASE("Test making sphere with invalid coordinates", "[scene]") {
    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": \"0\", \"y\": 0, "
                                             "\"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                             "\"lambert\": 1,"
                                             "\"radius\": 1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making sphere with negative radius", "[scene]") {
    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                             "\"lambert\": 1,"
                                             "\"radius\": -1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making sphere with lambert outside of [0, 1]", "[scene]") {
    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                             "\"lambert\": 1.1,"
                                             "\"radius\": 1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);

    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                             "\"lambert\": 1.1,"
                                             "\"radius\": 1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making sphere with color outside of [0, 255]", "[scene]") {
    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": -1, \"g\": 0, "
                                             "\"r\": 255},"
                                             "\"lambert\": 1,"
                                             "\"radius\": 1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);

    REQUIRE_THROWS_AS(Sphere(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"color\": "
                                             "{\"b\": 256, \"g\": 0, "
                                             "\"r\": 255},"
                                             "\"lambert\": 1,"
                                             "\"radius\": 1,"
                                             "\"type\": \"sphere\""
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making light with negative intensity", "[scene]") {
    REQUIRE_THROWS_AS(Light(strToJsonObject("{\"location\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"intensity\": -2"
                                            "}")),
                      JsonFormatError);
}

TEST_CASE("Test making a camera with a negative size", "[sceen]") {
    REQUIRE_THROWS_AS(Camera(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"focus\": 10,"
                                             "\"normal\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                             "\"resolution\": [0.01, 0.01],"
                                             "\"size\": [-256,256]"
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making a camera with a non-integral size", "[sceen]") {
    REQUIRE_THROWS_AS(Camera(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"focus\": 10,"
                                             "\"normal\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                             "\"resolution\": [0.01, 0.01],"
                                             "\"size\": [256.5,256]"
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making a camera with a negative resolution", "[sceen]") {
    REQUIRE_THROWS_AS(Camera(strToJsonObject("{\"center\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                             "\"focus\": 10,"
                                             "\"normal\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                             "\"resolution\": [-0.01, 0.01],"
                                             "\"size\": [256,256]"
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test making a camera with a non-object center", "[sceen]") {
    REQUIRE_THROWS_AS(Camera(strToJsonObject("{\"center\": "
                                             "[0, 0, 0],"
                                             "\"focus\": 10,"
                                             "\"normal\": "
                                             "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                             "\"resolution\": [-0.01, 0.01],"
                                             "\"size\": [256,256]"
                                             "}")),
                      JsonFormatError);
}

TEST_CASE("Test finding intersect of ray and sphere", "[scene]") {
    Ray r = Ray(Vec3D(0, 0, 0), Vec3D(0, 0, 1));
    Sphere s = Sphere(strToJsonObject("{\"center\": "
                                      "{ \"x\": 0, \"y\": 0, \"z\": 3},"
                                      "\"color\": "
                                      "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                      "\"lambert\": 1,"
                                      "\"radius\": 1,"
                                      "\"type\": \"sphere\""
                                      "}"));
    REQUIRE(s.intersect(r) == 2);
    r = Ray(Vec3D(0, 0, 0), Vec3D(0, 1, 0));
    REQUIRE(s.intersect(r) == std::numeric_limits<double>::infinity());
}

TEST_CASE("Test finding intersect of ray and plane", "[scene]") {
    Ray r = Ray(Vec3D(-1, 0, 0), Vec3D(1, 0, 0));
    Plane p = Plane(strToJsonObject("{\"center\": "
                                     "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                     "\"normal\": "
                                     "{ \"x\": 1, \"y\": 0, \"z\": 0},"
                                     "\"color\": "
                                     "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                     "\"lambert\": 1,"
                                     "\"type\": \"plane\""
                                     "}"));
    REQUIRE(p.intersect(r) == 1);
    r = Ray(Vec3D(-1, 0, 0), Vec3D(-1, 1, 0));
    REQUIRE(p.intersect(r) == std::numeric_limits<double>::infinity());
}

TEST_CASE("Test normalAt for Sphere", "[scene]") {
    Sphere s = Sphere(strToJsonObject("{\"center\": "
                                      "{ \"x\": 0, \"y\": 0, \"z\": 3},"
                                      "\"color\": "
                                      "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                      "\"lambert\": 1,"
                                      "\"radius\": 1,"
                                      "\"type\": \"sphere\""
                                      "}"));
    REQUIRE(s.normalAt(Vec3D(0, 1, 0)) == Vec3D(0, 1, 0));
}

TEST_CASE("Test normalAt for Plane", "[scene]") {
    Plane p = Plane(strToJsonObject("{\"center\": "
                                     "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                     "\"normal\": "
                                     "{ \"x\": 1, \"y\": 0, \"z\": 0},"
                                     "\"color\": "
                                     "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                     "\"lambert\": 1,"
                                     "\"type\": \"plane\""
                                     "}"));
    REQUIRE(p.normalAt(Vec3D(0, 0, 0)) == Vec3D(1, 0, 0));    
}

TEST_CASE("Making Scene with non-array objects", "[scene]") {
    REQUIRE_THROWS_AS(Scene(strToJsonObject("{\"camera\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"focus\": 10,"
                                            "\"normal\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                            "\"resolution\": [0.01, 0.01],"
                                            "\"size\": [256,256]},"
                                            "\"lights\":"
                                            "[{\"intensity\": 1,"
                                            "\"location\": "
                                            "{ \"x\": 5, \"y\": -5, \"z\": "
                                            "0}}],"
                                            "\"objects\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                            "\"color\": "
                                            "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                            "\"lambert\": 1,"
                                            "\"radius\": 1,"
                                            "\"type\": \"sphere\"}}")),
                      JsonFormatError);
}

TEST_CASE("Making Scene with non sphere or plane object", "[scene]") {
    REQUIRE_THROWS_AS(Scene(strToJsonObject("{\"camera\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"focus\": 10,"
                                            "\"normal\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                            "\"resolution\": [0.01, 0.01],"
                                            "\"size\": [256,256]},"
                                            "\"lights\":"
                                            "[{\"intensity\": 1,"
                                            "\"location\": "
                                            "{ \"x\": 5, \"y\": -5, \"z\": "
                                            "0}}],"
                                            "\"objects\":"
                                            "[{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                            "\"color\": "
                                            "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                            "\"lambert\": 1,"
                                            "\"radius\": 1,"
                                            "\"type\": \"other\"}]}")),
                      JsonFormatError);
}

TEST_CASE("Making Scene with non array lights", "[scene]") {
    REQUIRE_THROWS_AS(Scene(strToJsonObject("{\"camera\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"focus\": 10,"
                                            "\"normal\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                            "\"resolution\": [0.01, 0.01],"
                                            "\"size\": [256,256]},"
                                            "\"lights\":"
                                            "{\"intensity\": 1,"
                                            "\"location\": "
                                            "{ \"x\": 5, \"y\": -5, \"z\": "
                                            "0}},"
                                            "\"objects\":"
                                            "[{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                            "\"color\": "
                                            "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                            "\"lambert\": 1,"
                                            "\"radius\": 1,"
                                            "\"type\": \"sphere\"}]}")),
                      JsonFormatError);
}

TEST_CASE("Making Scene with non-object light", "[scene]") {
    REQUIRE_THROWS_AS(Scene(strToJsonObject("{\"camera\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"focus\": 10,"
                                            "\"normal\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                            "\"resolution\": [0.01, 0.01],"
                                            "\"size\": [256,256]},"
                                            "\"lights\":"
                                            "[[], {\"intensity\": 1,"
                                            "\"location\": "
                                            "{ \"x\": 5, \"y\": -5, \"z\": "
                                            "0}}],"
                                            "\"objects\":"
                                            "[{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                            "\"color\": "
                                            "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                            "\"lambert\": 1,"
                                            "\"radius\": 1,"
                                            "\"type\": \"sphere\"}]}")),
                      JsonFormatError);
}

TEST_CASE("Making Scene with non-object object", "[scene]") {
    REQUIRE_THROWS_AS(Scene(strToJsonObject("{\"camera\":"
                                            "{\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                            "\"focus\": 10,"
                                            "\"normal\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                            "\"resolution\": [0.01, 0.01],"
                                            "\"size\": [256,256]},"
                                            "\"lights\":"
                                            "[{\"intensity\": 1,"
                                            "\"location\": "
                                            "{ \"x\": 5, \"y\": -5, \"z\": "
                                            "0}}],"
                                            "\"objects\":"
                                            "[[], {\"center\": "
                                            "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                            "\"color\": "
                                            "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                            "\"lambert\": 1,"
                                            "\"radius\": 1,"
                                            "\"type\": \"sphere\"}]}")),
                      JsonFormatError);
}

TEST_CASE("Test Ray.pointAt()", "[ray]") {
    Ray r = Ray(Vec3D(0, 0, 0), Vec3D(2, 0, 0));
    REQUIRE(r.pointAt(3) == Vec3D(3, 0, 0));
    REQUIRE(r.pointAt(1.5) == Vec3D(1.5, 0, 0));
    REQUIRE(r.pointAt(-3) == Vec3D(-3, 0, 0));

    r = Ray(Vec3D(1, 1, 1), Vec3D(4, 1, 5));
    REQUIRE(r.pointAt(5) == Vec3D(4, 1, 5));
    REQUIRE(r.pointAt(-5) == Vec3D(-2, 1, -3));
}

TEST_CASE("Test rotating rays", "[ray]") {
    Vec3D v = Vec3D(1, 0, 0);
    Vec3D axis = Vec3D(0, 1, 0);
    REQUIRE(rotate(v, axis, 0) == v);
    REQUIRE(rotate(v, axis, M_PI/2) == Vec3D(0, 0, -1));
    REQUIRE(rotate(v, axis, M_PI) == Vec3D(-1, 0, 0));
}

TEST_CASE("Test Color Equality", "[color]") {
    REQUIRE(Color(50, 10, 20) == Color(50, 10, 20));
}

TEST_CASE("Test color adding (+)", "[color]") {
    REQUIRE(Color(50, 10, 20) + Color(10, 10, 10) == Color(60, 20, 30));
}

TEST_CASE("Test color incrementing (+=)", "[color]") {
    Color a = Color(50, 10, 20);
    a += Color(10, 10, 10);
    REQUIRE(a == Color(60, 20, 30));
}

TEST_CASE("Test color scaling (*)", "[color]") {
    REQUIRE(Color(50, 10, 20) * 2 == Color(100, 20, 40));
    REQUIRE(2 * Color(50, 10, 20) == Color(100, 20, 40));
    REQUIRE(Color(50, 10, 20) * 0.5 == Color(25, 5, 10));
    REQUIRE(0.5 * Color(50, 10, 20) == Color(25, 5, 10));
}

TEST_CASE("Test Color scaling (*=)", "[color]") {
    Color a = Color(50, 10, 20);
    a *= 2;
    REQUIRE(a == Color(100, 20, 40));
    a *= 0.25;
    REQUIRE(a == Color(25, 5, 10));
}

TEST_CASE("Test Color max", "[color]") {
    REQUIRE(Color(100, 20, 40).max() == 100);
    REQUIRE(Color(0, 0, 0).max() == 0);
    REQUIRE(Color(100, 200, 40).max() == 200);
    REQUIRE(Color(100, 20, 400).max() == 400);
}

TEST_CASE("Test Color default constructor", "[color]") {
    REQUIRE(Color() == Color(0, 0, 0));
}

TEST_CASE("Run Tracer", "[tracer]") {
    Scene s = Scene(strToJsonObject("{\"camera\":"
                                    "{\"center\": "
                                    "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                    "\"focus\": 10,"
                                    "\"normal\": "
                                    "{ \"x\": 0, \"y\": 0, \"z\": 1},"
                                    "\"resolution\": [0.01, 0.01],"
                                    "\"size\": [256,256]},"
                                    "\"lights\":"
                                    "[{\"intensity\": 1,"
                                    "\"location\": "
                                    "{ \"x\": 5, \"y\": -5, \"z\": 0}}],"
                                    "\"objects\":"
                                    "[{\"center\": "
                                    "{ \"x\": 0, \"y\": 0, \"z\": 5},"
                                    "\"color\": "
                                    "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                    "\"lambert\": 1,"
                                    "\"radius\": 1,"
                                    "\"type\": \"sphere\"},"
                                    "{\"center\": "
                                    "{ \"x\": 0, \"y\": 0, \"z\": 0},"
                                    "\"normal\": "
                                    "{ \"x\": 1, \"y\": 0, \"z\": 0},"
                                    "\"color\": "
                                    "{\"b\": 0, \"g\": 0, \"r\": 255},"
                                    "\"lambert\": 1,"
                                    "\"type\": \"plane\""
                                    "}]}"));
    REQUIRE_NOTHROW(trace(&s, 128, 128));
}

TEST_CASE("Test Image Normalization", "[image_write]") {
    Color ** list = new Color*[5];
    list[0] = new Color(0, 0, 0);
    list[1] = new Color(1, 100, 1000);
    list[2] = new Color(100, 100, 100);
    list[3] = new Color(50, 999, 980);
    list[4] = new Color(1, 1, 1);
    normalizeImage(list, 5);
    for (size_t i = 0; i < 5; i++) {
        REQUIRE(list[i]->max() <= 255);
    }

    for (size_t i = 0; i < 5; i++) {
        delete list[i];
    }
    delete[] list;
}
