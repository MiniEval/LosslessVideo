# ZStd-based Lossless Video Playback

Image sequence compiler and player for lossless video in a Zstd-based data format. Capable of playback at 1000+MB/s worth of raw image data, equivalent to around 120FPS at 1080p, or 30FPS at 4K.

## Dependencies

- ZStd: https://github.com/facebook/zstd
- SDL2: https://github.com/libsdl-org/SDL
- SDL2_image: https://github.com/davidsiaw/SDL2_image

## Usage

Compiler: LV_Compiler.exe [IMAGE SEQUENCE FOLDER]

Video Player: LV_Player.exe [.ZS FILE] [FRAME RATE (INTEGER)]
