//
// Created by the_z on 26/04/2021.
//

#ifndef GL_IMAGE_H
#define GL_IMAGE_H

#include <string>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

void invertSurface(SDL_Surface *surface);
SDL_Surface *loadImage(const std::string &filePath);

#endif //GL_IMAGE_H
