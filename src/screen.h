#ifndef GL_SCREEN_H
#define GL_SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

#include "graphics/shader.h"
#include "graphics/texture.h"

class Screen {
public:
    Screen(std::pair<int, int> &viewPort, std::pair<int, int> &viewPortVirtual, const std::string &title);
    ~Screen();

    void resize();
    void flip(Shader &postShader, int ditherHandle);

    GLFWwindow *window;

    std::pair<int, int> &viewPort;
    std::pair<int, int> &viewPortVirtual;

private:
    Shader *copyShader;

    unsigned int fbo[2];
    unsigned int rbo[2];
    unsigned int fboTexHandle[2];

    unsigned int fboQuadVao;
};

#endif // GL_SCREEN_H
