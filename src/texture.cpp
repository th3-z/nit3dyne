//
// Created by the_z on 26/04/2021.
//

#include "texture.h"

unsigned int Texture::cHandle = 1;

Texture::Texture(std::string &type, std::string &filePath):
handle(cHandle++),
type(type){
    int w, h, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath.c_str(), &w, &h, &nChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture: " << filePath << std::endl;
    }

    int mode = GL_RGB;
    if (nChannels == 4)
        mode = GL_RGBA;

    glGenTextures(1, &this->handle);
    glBindTexture(GL_TEXTURE_2D, this->handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, mode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->handle);
}


