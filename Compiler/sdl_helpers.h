#ifndef SDL_HELPERS
#define SDL_HELPERS

#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>

SDL_Surface* load_png_image(std::filesystem::path path);

#endif