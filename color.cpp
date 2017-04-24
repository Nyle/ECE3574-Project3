#include "color.hpp"

const Color Color::operator*(double d) const {
    return Color(d * r, d * g, d * b);
}

size_t Color::max() const {
    size_t tmp = r > g ? r : g;
    return b > tmp ? b : tmp;
}

const Color operator*(double d, const Color c) {
    return c * d;
}
