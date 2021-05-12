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
    unsigned int handle;
    std::string type;

    Texture(std::string &type, std::string &filePath);
    ~Texture();
};


#endif //GL_TEXTURE_H
