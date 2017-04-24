#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "scene.hpp"
#include "json_format_error.hpp"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

QJsonDocument _strToJsonDocument(std::string str) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(str),
                                                &error);
    REQUIRE(error.error == QJsonParseError::NoError);
    return doc;
}

QJsonObject strToJsonObject(std::string str) {
    QJsonDocument doc = _strToJsonDocument(str);
    REQUIRE(doc.isObject());
    return doc.object();
}

QJsonArray strToJsonArray(std::string str) {
    QJsonDocument doc = _strToJsonDocument(str);
    REQUIRE(doc.isArray());
    return doc.array();
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
                                           "{ \"x\": 0, \"y\": 0, \"z\": 1},"
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
                                          "\"type\": \"sphere\"}]}")));
}
