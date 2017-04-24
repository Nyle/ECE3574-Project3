#include "color.hpp"

const Color Color::operator*(double d) const {
    return Color(d * r, d * g, d * b);
}

const Color Color::operator+=(const Color c) {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

size_t Color::max() const {
    size_t tmp = r > g ? r : g;
    return b > tmp ? b : tmp;
}

inline Color operator+(Color lhs, const Color& rhs) {
    lhs += rhs;
    return lhs;
}

const Color operator*(double d, const Color c) {
    return c * d;
}
