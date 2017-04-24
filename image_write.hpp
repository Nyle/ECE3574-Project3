#ifndef IMAGE_WRITE_HPP
#define IMAGE_WRITE_HPP

#include <vector>
#include <string>
class Color;

void normalizeImage(Color ** imageData, size_t size);

void outputImage(Color ** imageData, std::string file,
                 size_t w, size_t h);

#endif // IMAGE_WRITE_HPP
