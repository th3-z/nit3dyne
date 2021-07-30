#ifndef GL_DISPLAY_H
#define GL_DISPLAY_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "utils/rand.h"

namespace n3d {

class Display {
public:
    inline static std::pair<int, int> viewPort;
    inline static std::pair<int, int> viewPortVirtual;

    inline static std::string title;
    inline static GLFWwindow *window;
    inline static bool shouldClose;

    inline static long frame;
    inline static double timeDelta;
    inline static double target_frametime;

    static void init();

    static void destroy();

    static void update();

    static void flip(Shader &postShader);

private:
    inline static double timeLastFrame;
    inline static double timeThisFrame;

    inline static Shader *copyShader;
    inline static Texture *dither;

    inline static unsigned int fbo[2];
    inline static unsigned int rbo[2];
    inline static unsigned int fboTexHandle[2];
    inline static unsigned int fboQuadVao;


    static void initGlfw();

    static void initGl();

    static void initBuffers();

    static void initResources();
};

}

#endif //GL_DISPLAY_H