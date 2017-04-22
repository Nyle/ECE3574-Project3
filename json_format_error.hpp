#ifndef JSON_FORMAT_ERROR_HPP
#define JSON_FORMAT_ERROR_HPP

#include <exception>
#include <stdexcept>

class JsonFormatError : public std::runtime_error {
public:
  JsonFormatError(const std::string& message) : std::runtime_error(message){};
};

#endif // JSON_FORMAT_ERROR_HPP
