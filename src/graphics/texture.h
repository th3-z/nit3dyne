#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <string>

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

#endif // GL_TEXTURE_H
