//
// Created by the_z on 26/04/2021.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H


#include <string>
#include <glad/glad.h>
#include "util/image.h"

class Texture {
public:
    unsigned int handle;
    std::string type;

    Texture(std::string &type, std::string &filePath);
    ~Texture();
private:
    static unsigned int cHandle;
};


#endif //GL_TEXTURE_H
