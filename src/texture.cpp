//
// Created by the_z on 26/04/2021.
//

#include "texture.h"

unsigned int Texture::cHandle = 1;

Texture::Texture(std::string &type, std::string &filePath):
handle(cHandle++),
type(type){
    SDL_Surface *image = loadImage(filePath);

    int mode = GL_RGB;
    if (image->format->BytesPerPixel == 4)
        mode = GL_RGBA;

    glGenTextures(1, &this->handle);
    glBindTexture(GL_TEXTURE_2D, this->handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->handle);
}


