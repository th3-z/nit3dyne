#ifndef GL_SKYBOX_H
#define GL_SKYBOX_H

#include <string>
#include <vector>

#include "stb_image.h"
#include <cassert>
#include <glad/glad.h>

#include "shader.h"

class Skybox {
public:
    // faceFilePaths should always be of size six
    Skybox(std::vector<std::string> faceFilePaths);
    ~Skybox();

    void draw(Shader &shader, glm::mat4 &&view, glm::mat4 &proj);

private:
    unsigned int handle;
    unsigned int VAO;
};

#endif // GL_SKYBOX_H
