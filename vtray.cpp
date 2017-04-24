#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "scene.hpp"
#include "tracer.hpp"
#include "image_write.hpp"

int main(int argc, char ** argv) {
    if (argc != 3) {
        std::cerr << "Error: Please provide input and output file";
        return EXIT_FAILURE;
    }
    Scene * scene = loadSceneFromFile(argv[1]);
    Color ** image = new Color*[scene->camera.sizey * scene->camera.sizex];
    size_t w = scene->camera.sizex;
    size_t h = scene->camera.sizey;
    for (size_t j = 0; j < h; j++) {
        for (size_t i = 0; i < w; i++) {
            image[j * w + i] = trace(scene, i, j);
        }
    }
    normalizeImage(image, w = h);
    outputImage(image, argv[2], w, h);
    return EXIT_SUCCESS;
}

