#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>

#include "scene.hpp"
#include "tracer.hpp"
#include "image_write.hpp"

int main(int argc, char * argv[]) {
    std::vector<std::string>  arguments;
    for(int i = 0; i < argc; ++i) arguments.push_back(argv[i]);
    
    size_t numthreads = 1;
    std::string infile, outfile;
    for (size_t i = 1; i < arguments.size(); i++) {
        if (arguments[i].substr(0, 2) == "-t") {
            if (arguments.size() == 5 && i < arguments.size() - 1) {
                numthreads = atoi(arguments[i+1].c_str());
                arguments.erase(arguments.begin() + i,
                                arguments.begin() + i + 2);
                break;
            } else if (arguments.size() == 4) {
                numthreads = atoi(arguments[i+1].substr(2).c_str());
                arguments.erase(arguments.begin() + i);
                break;
            }
        }
    }

    if (arguments.size() != 3) {
        std::cerr << "Error: Malformed command line arguments" << std::endl;
        return EXIT_FAILURE;
    }
    infile = arguments[1];
    outfile = arguments[2];
    Scene * scene = loadSceneFromFile(argv[1]);
    size_t w = scene->camera.sizex;
    size_t h = scene->camera.sizey;
    size_t pixels = w * h;
    Color ** image = new Color*[pixels];

    std::atomic<size_t> currentPixel(0);
    std::vector<std::thread*> threads;
    
    for (size_t i = 0; i < numthreads; i++) {
        std::thread * worker = new std::thread([&] () {
                size_t pixel;
                while (true) {
                    pixel = currentPixel++;
                    if (pixel >= pixels) return;
                    image[pixel] = trace(scene, pixel % w, pixel / w);
                }
            });
        threads.push_back(worker);
    }

    for (auto thread : threads) {
        thread->join();
    }
    
    normalizeImage(image, w * h);
    outputImage(image, argv[2], w, h);
    return EXIT_SUCCESS;
}
