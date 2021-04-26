//
// Created by the_z on 26/04/2021.
//


#include "image.h"

void invertSurface(SDL_Surface *surface) {
    SDL_LockSurface(surface);

    int pitch = surface->pitch; // row size
    char* temp = new char[pitch]; // intermediate buffer
    char* pixels = (char*) surface->pixels;

    for (int i = 0; i < surface->h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (surface->h - i - 1) * pitch;

        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }

    delete[] temp;

    SDL_UnlockSurface(surface);
}

SDL_Surface *loadImage(const std::string &filePath) {
    SDL_Surface *image;

    std::string ext = filePath.substr(
        filePath.find(std::string("."))
    );
    std::transform(
        ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

    if (!strcmp(ext.c_str(), ".jpg") | !strcmp(ext.c_str(), ".jpeg")) {
        IMG_Init(IMG_INIT_JPG);
        image = IMG_Load(filePath.c_str());
    } else if (!strcmp(ext.c_str(), ".png")) {
        IMG_Init(IMG_INIT_PNG);
        image = IMG_Load(filePath.c_str());
    } else if (!strcmp(ext.c_str(), ".bmp")) {
        image = SDL_LoadBMP(filePath.c_str());
    } else {
        std::cout << "ERROR: Unrecognised image extension:" << ext << std::endl;
    }

    if (image == nullptr)
        std::cout << SDL_GetError() << std::endl;

    invertSurface(image);

    IMG_Quit();

    return image;
}