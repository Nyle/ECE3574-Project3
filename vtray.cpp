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
    std::vector<Color> image;

    for (size_t j = 0; j < scene->camera.sizey; j++) {
        for (size_t i = 0; i < scene->camera.sizex; i++) {
            image.push_back(trace(scene, i, j));
        }
    }

    outputImage(image, argv[2], scene->camera.sizex, scene->camera.sizey);
    
    return EXIT_SUCCESS;
}

