#include "color.hpp"

const Color Color::operator*=(double d) {
    r *= d;
    g *= d;
    b *= d;
    return *this;
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

bool Color::operator==(const Color c) {
    return r == c.r && g == c.g && b == c.b;
}

Color operator+(Color lhs, const Color& rhs) {
    lhs += rhs;
    return lhs;
}

Color operator*(Color lhs, double rhs) {
    lhs *= rhs;
    return lhs;
}

Color operator*(double lhs, const Color& rhs) {
    return rhs * lhs;
}
