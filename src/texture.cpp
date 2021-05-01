//
// Created by the_z on 26/04/2021.
//

#include "texture.h"

unsigned int Texture::cId = 0;

Texture::Texture(std::string &filePath):
id(cId++) {
    SDL_Surface *image = loadImage(filePath);


    int mode = GL_RGB;
    if (image->format->BytesPerPixel == 4)
        mode = GL_RGBA;

    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);


    glBindTexture(GL_TEXTURE_2D, 0);
}
