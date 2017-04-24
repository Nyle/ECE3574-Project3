#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdlib>

class Color {
public:
    size_t r, g, b;

    // Constructor from values;
    Color(size_t r, size_t g, size_t b) : r(r), g(g), b(b) {}

    const Color operator*(double d) const;

    const Color operator+=(const Color c);

    const Color operator+(const Color c) const;

    size_t max() const;
};

inline Color operator+(Color lhs, const Color& rhs);

const Color operator*(double d, const Color c);

const Color VOID_COLOR = Color(0, 0, 0);

#endif
