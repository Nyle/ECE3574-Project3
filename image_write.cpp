#include "image_write.hpp"

#include "scene.hpp"
#include "QImage"

void normalizeImage(std::vector<Color> &imageData) {
    size_t max = 0;
    size_t tmp;

    for (Color c : imageData) {
        tmp = c.max();
        max = tmp > max ? tmp : max;
    }

    double scaleFactor = 250.0 / max;

    for (auto&& c : imageData) {
        c = c * scaleFactor;
    }
}

void outputImage(std::vector<Color> imageData, std::string file,
                 size_t w, size_t h) {
    QImage image = QImage(w, h, QImage::Format_RGB32);
    for (size_t i = 0; i < imageData.size(); i++) {
        Color c = imageData[i];
        image.setPixel(i % w, i / w, qRgb(c.r, c.g, c.b));
    }
    image.save(file.c_str());
}
