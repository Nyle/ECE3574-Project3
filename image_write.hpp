#ifndef IMAGE_WRITE_HPP
#define IMAGE_WRITE_HPP

#include <vector>
#include <string>
class Color;

void outputImage(std::vector<Color> imageData, std::string file,
                 size_t w, size_t h);

#endif // IMAGE_WRITE_HPP
