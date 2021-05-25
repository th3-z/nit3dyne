#ifndef GL_INPUT_H
#define GL_INPUT_H

#include "camera/camera.h"
#include "camera/cameraFps.h"
#include "camera/cameraFree.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

struct WindowState {
    double timeDelta = 0.;
    std::unique_ptr<Camera> camera;
};

class Input {
public:
    static void processContinuousInput(GLFWwindow *window);
    static void callbackKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void callbackMouse(GLFWwindow *window, double mouseX, double mouseY);
    static void callbackScroll(GLFWwindow *window, double xOffset, double yOffset);
    static void registerCallbacks(GLFWwindow *window);

private:
    static double mouseLastX;
    static double mouseLastY;
};

#endif // GL_INPUT_H
