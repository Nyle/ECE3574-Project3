#ifndef TRACER_HPP
#define TRACER_HPP

#include <cstdlib>
class Color;
class Scene;

// Trace the pixel at coordinate i, j
Color * trace(const Scene &s, size_t i, size_t j);

#endif // TRACER_HPP
