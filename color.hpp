#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdlib>

class Color {
public:
    size_t r, g, b;

    // Constructor from values;
    Color(size_t r, size_t g, size_t b) : r(r), g(g), b(b) {}

    const Color operator*(double d) const;

    size_t max() const;
};

const Color operator*(double d, const Color c);

const Color VOID_COLOR = Color(0, 0, 0);

#endif
