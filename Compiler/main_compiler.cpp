#include "sdl_helpers.h"
#include <zstd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void compile_images(std::filesystem::path path) {
    std::vector<std::filesystem::path> image_paths;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if(entry.path().extension().string().compare(".png") == 0) {
            image_paths.push_back(entry.path());
        }
    }

    std::filesystem::path compile_dir = path;
    compile_dir.append("compile");
    std::filesystem::create_directory(compile_dir);

    std::stringstream* compressed = new std::stringstream[image_paths.size()];
    int width = -1;
    int height = -1;
    int pitch = -1;
    Uint32 format = SDL_PIXELFORMAT_ARGB8888;

    for (size_t i = 0; i < image_paths.size(); i++) {
        std::cout << image_paths[i] << std::endl;
        SDL_Surface* image = load_png_image(image_paths[i]);
        if (image == NULL) {
            std::cout << SDL_GetError() << std::endl;
            exit(1);
        }
        image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);

        SDL_LockSurface(image);

        if (width == -1) {
            width = image->w;
            height = image->h;
            pitch = image->pitch;

            printf("%d, %d, %d, %d\n", width, height, pitch, format);
        }
        else {
            if (image->w != width && image->h != height && image->pitch != pitch && image->format->format != format) {
                std::cout << "Image size and/or format are inconsistent! Terminating..." << std::endl;
                exit(1);
            }
        }

        size_t input_length = (size_t)pitch * height;
        unsigned char* input = (unsigned char*)image->pixels;
        size_t bound = ZSTD_compressBound(input_length);
        unsigned char* output = new unsigned char[bound];

        int output_size = (int)ZSTD_compress(output, bound, input, input_length, 18);
        std::cout << input_length << " " << output_size << std::endl;

        compressed[i].write((char*)&output_size, sizeof(int));
        compressed[i].write((char*)output, output_size);

        delete[] output;
        SDL_FreeSurface(image);
    }

    std::filesystem::path file_dir = compile_dir;
    file_dir.append("sequence.zs");
    std::ofstream file;
    file.open(file_dir, std::ios::binary);

    file.write((char*)&width, sizeof(int));
    file.write((char*)&height, sizeof(int));
    file.write((char*)&pitch, sizeof(int));
    file.write((char*)&format, sizeof(Uint32));
    printf("%d, %d, %d, %d", width, height, pitch, format);
    for (size_t i = 0; i < image_paths.size(); i++) {
        compressed[i].seekg(0, std::ios::end);
        size_t size = compressed[i].tellg();
        compressed[i].seekg(0, std::ios::beg);

        char* contents = new char[size];
        compressed[i].read(contents, size);

        file.write(contents, size);
        
        delete[] contents;
    }
    file.close();
    delete[] compressed;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "USAGE: " << argv[1] << " [FOLDER PATH]" << std::endl;
        return 1;
    }
    std::cout << argv[1] << std::endl;
    std::filesystem::path path(argv[1]);
    std::error_code ec;
    if (std::filesystem::is_directory(path, ec)) {
        compile_images(path);
    } else {
        std::cout << "ERROR: Path is not directory or doesn't exist." << std::endl;
        return 1;
    }
    return 0;
}