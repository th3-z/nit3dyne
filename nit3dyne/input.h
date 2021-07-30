#ifndef GL_INPUT_H
#define GL_INPUT_H

#include "camera/camera.h"
#include "camera/cameraFps.h"
#include "camera/cameraFree.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace n3d {

enum Direction {
    UP = 1, LEFT = 2, DOWN = 4, RIGHT = 8, FORWARD = 16, BACKWARD = 32
};

class Input {
public:
    inline static std::pair<double, double> mousePos;
    inline static std::pair<double, double> mousePosDelta;
    inline static std::pair<double, double> scrollDelta;
    inline static int direction;

    // Pointer owned by Display
    static void init(GLFWwindow *glfwWindow);

    static void update();

    static bool getKey(int scanCode);

    static void callbackMouse(GLFWwindow *window, double mouseX, double mouseY);

    static void callbackScroll(GLFWwindow *window, double scrollX, double scrollY);

    static void registerCallbacks();

private:
    inline static double mouseLastX;
    inline static double mouseLastY;

    // Pointer owned by display
    inline static GLFWwindow *window;
};

}
#endif // GL_INPUT_H
