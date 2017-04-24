#include "input_reader.hpp"

#include <QByteArray>
#include <QJsonParseError>
#include <QJsonArray>

InputReader::InputReader(std::string input) {
    wasError = false;
    
    QJsonParseError error;
    QByteArray inputAsByteArray = QByteArray::fromStdString(input);
    QJsonDocument document = QJsonDocument::fromJson(inputAsByteArray,
                                                     &error);
    if (error.error != QJsonParseError::NoError) {
        setError(error.errorString().toStdString());
        return;
    }

    if (!document.isObject()) {
        setError("Expected top level to be an object");
        return;
    }

    if (!readObjects(document.object())) return;
    
}

bool InputReader::readObjects(QJsonObject jScene) {
    if (jScene["objects"] == QJsonValue::Undefined) {
        setError("Expected 'objects' key to be present in top level object");
        return false;
    }
    if (!jScene["objects"].isArray()) {
        setError("Expected 'objects' key to hold array");
        return false;
    }
    QJsonArray jObjects = jScene["objects"].toArray();
    for (auto obj = jObjects.begin(); obj != jObjects.end(); obj++) {
        
        objects.push_back(
    }
    
}
