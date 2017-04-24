#include <string>
#include <fstream>
#include <iostream>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>


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



int main() {
    std::string filename = "tests/scene0.json";
    std::ifstream file(filename);
    std::cout << readFile(file);
}
