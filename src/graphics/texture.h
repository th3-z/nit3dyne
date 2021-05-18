//
// Created by the_z on 26/04/2021.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>

#include "stb_image.h"

class Texture {
public:
    explicit Texture(const std::string &filePath);
    ~Texture();

    unsigned int handle;
    int channels;
    int w;
    int h;
};


#endif //GL_TEXTURE_H
