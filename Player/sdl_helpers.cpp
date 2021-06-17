#include "sdl_helpers.h"
#include <zstd.h>
#include <iostream>

SDL_Texture* image = NULL;
size_t pixelbuffersize = 0;

void initSDL(SDL_Window** window, SDL_Renderer** renderer, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Couldn't initialise SDL" << std::endl;
		exit(1);
	}

	*window = SDL_CreateWindow("Lossless Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (*window == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer == NULL) {
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void prepareImage(SDL_Renderer* renderer, ImageSet* imgs, int idx) {
	if (!image) {
		printf("%d, %d, %d, %d\n", imgs->width, imgs->height, imgs->pitch, imgs->format);
		image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, imgs->width, imgs->height);
		if (!image) {
			std::cout << "Couldn't create texture!" << std::endl;
			exit(1);
		}
		pixelbuffersize = (size_t)imgs->pitch * imgs->height;
		std::cout << pixelbuffersize << std::endl;
	}

	void* pixels;
	int pitch;
	SDL_LockTexture(image, NULL, (void**) &pixels, &pitch);
	size_t img_size = ZSTD_decompress(pixels, pixelbuffersize, imgs->image_data[idx], (size_t)imgs->compressed_size[idx]);
	SDL_UnlockTexture(image);
}

void presentImage(SDL_Renderer* renderer) {
	int err;
	err = SDL_RenderClear(renderer);
	if (err != 0) {
		std::cout << SDL_GetError() << std::endl;
	}
	err = SDL_RenderCopy(renderer, image, NULL, NULL);
	if (err != 0) {
		std::cout << SDL_GetError() << std::endl;
	}
	SDL_RenderPresent(renderer);
}