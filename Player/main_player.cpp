#include "sdl_helpers.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <string>

void parse_file(std::filesystem::path path, ImageSet* imgs) {
    std::ifstream file;

    file.open(path, std::ios::binary);
    
    int format = 0;
    file.read((char*)&(imgs->width), sizeof(int));
    file.read((char*)&(imgs->height), sizeof(int));
    file.read((char*)&(imgs->pitch), sizeof(int));
    file.read((char*)&(imgs->format), sizeof(Uint32));

    while (!file.eof()) {
        int compressed_size;
        file.read((char*)&compressed_size, sizeof(int));

        if (!file.eof()) {
            void* data = new char[compressed_size];
            file.read((char*)data, compressed_size);
            imgs->image_data.push_back(data);
            imgs->compressed_size.push_back(compressed_size);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "USAGE: " << argv[1] << " [FOLDER PATH] [FRAME RATE (INTEGER)]" << std::endl;
        return 1;
    }
    std::cout << argv[1] << std::endl;
    std::filesystem::path path(argv[1]);
    int framerate = std::atoi(argv[2]);

    if (std::filesystem::exists(path)) {
        ImageSet imgs;

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        parse_file(path, &imgs);
        std::cout << imgs.image_data.size() << std::endl;

        initSDL(&window, &renderer, imgs.width, imgs.height);

        int idx = 0;
        bool quit = false;
        std::chrono::system_clock::time_point last_tick = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point current_tick;
        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                /* handle your event here */

               //User requests quit
                switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                }
            }

            current_tick = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = current_tick - last_tick;

            if (diff.count() > 1.0 / framerate) {
                last_tick = std::chrono::system_clock::now();
                prepareImage(renderer, &imgs, idx);
                presentImage(renderer);

                idx += 1;
                if (idx >= imgs.image_data.size()) {
                    idx = 0;
                }
            }
            SDL_Delay(1);
        }
    }
    else {
        std::cout << "File does not exist." << std::endl;
        return 1;
    }

	return 0;
}