#include "sdl_helpers.h"
#include <iostream>

SDL_Surface* load_png_image(std::filesystem::path path) {
    SDL_Surface* image;
    image = IMG_Load(path.string().c_str());
    if(!image) {
        std::cout << "ERROR: PNG image " << path.string() << " cannot be read." << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_LockSurface(image);
    return image;
}
