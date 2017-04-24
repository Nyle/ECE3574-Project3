#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include "scene.hpp"

#include <istream>
#include <QJsonDocument>
#include <QJsonObject>

class InputReader {
private:
    std::string errorMessage;
    bool wasError;
    std::vector<SceneObject> objects;
    std::vector<Light> lights;
    Camera camera;

    void setError(std::string error);
    bool readObjects(QJsonObject scene);
public:
    // Initialize the InputReader with the given string
    InputReader(std::string input);
};

#endif // INPUT_READER_HPP
