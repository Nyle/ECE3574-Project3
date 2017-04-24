#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdlib>

class Color {
public:
    size_t r, g, b;

    // Default constructor; Black
    Color() : r(0), g(0), b(0) {}
    // Constructor from values;
    Color(size_t r, size_t g, size_t b) : r(r), g(g), b(b) {}

    const Color operator*=(double d);
    const Color operator+=(const Color c);

    size_t max() const;

    bool operator==(const Color c);
};

Color operator+(Color lhs, const Color& rhs);
Color operator*(Color lhs, double rhs);
Color operator*(double lhs, const Color& rhs);

#endif
