#ifndef SDL_HELPERS
#define SDL_HELPERS

#include <SDL.h>
#include <vector>

typedef struct {
    int width;
    int height;
    int pitch;
    Uint32 format;
    std::vector<void*> image_data;
    std::vector<int> compressed_size;
} ImageSet;

void initSDL(SDL_Window** window, SDL_Renderer** renderer, int width, int height);

void prepareImage(SDL_Renderer* renderer, ImageSet* imgs, int idx);
void presentImage(SDL_Renderer* renderer);

#endif
