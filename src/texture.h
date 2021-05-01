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
    unsigned int id;

    Texture(std::string &filePath);
private:
    static unsigned int cId;
};


#endif //GL_TEXTURE_H
