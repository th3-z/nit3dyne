#ifndef GL_SKYBOX_H
#define GL_SKYBOX_H

#include <string>
#include <vector>
#include <array>
#include <cassert>
#ifndef NDEBUG
#include <iostream>
#endif

#include <stb_image.h>
#include <glad/glad.h>

#include "nit3dyne/graphics/shader.h"

namespace n3d {

class Skybox {
public:
    Skybox(const std::string &resourceName);

    ~Skybox();

    void draw(Shader &shader, glm::mat4 &&view, glm::mat4 &proj);

private:
    unsigned int handle;
    unsigned int VAO;
};

}

#endif // GL_SKYBOX_H
