#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <glad/glad.h>
#include <string>
#ifndef NDEBUG
#include <iostream>
#endif

#include <stb_image.h>
#include "nit3dyne/graphics/material.h"

namespace n3d {

class Texture {
public:
    explicit Texture(const std::string &resourceName);

    ~Texture();

    unsigned int handle;
    int channels;
    int w;
    int h;
};

}

#endif // GL_TEXTURE_H
