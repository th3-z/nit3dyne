#include "input.h"

namespace n3d {

void Input::callbackMouse(GLFWwindow *window, double mouseX, double mouseY) {
//    mousePos.first = mouseX;
//    mousePos.second = mouseY;
//
//    mousePosDelta.first = mouseX - mouseLastX;
//    mousePosDelta.second = mouseY - mouseLastY;
//
//    mouseLastX = mouseX;
//    mouseLastY = mouseY;
}

void Input::callbackScroll(GLFWwindow *window, double scrollX, double scrollY) {
    scrollDelta.first = scrollX;
    scrollDelta.second = scrollY;
}


//void Input::callbackKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
//
//    auto *windowState = (WindowState *) glfwGetWindowUserPointer(window);
//
//    if (key == GLFW_KEY_H && action == GLFW_PRESS)
//        windowState->ca = true;
//
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
//        // delete windowState->camera;
//        windowState->camera =
//            std::make_unique<CameraFps>(windowState->camera->fov, windowState->camera->viewPort);
//    }
//    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
//        windowState->camera =
//            std::make_unique<CameraFree>(windowState->camera->fov, windowState->camera->viewPort);
//    }
//}

void Input::registerCallbacks() {
    glfwSetCursorPosCallback(window, callbackMouse);
    glfwSetScrollCallback(window, callbackScroll);
}

bool Input::getKey(int scanCode) {
    return glfwGetKey(window, scanCode) == GLFW_PRESS;
}

void Input::init(GLFWwindow *glfwWindow) {
    window = glfwWindow;
    registerCallbacks();
}

void Input::update() {
    mousePosDelta.first = 0;
    mousePosDelta.second = 0;

    scrollDelta.first = 0;
    scrollDelta.second = 0;

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);
    glfwGetCursorPos(window, &mousePosDelta.first, &mousePosDelta.second);
    mousePosDelta.first -= winW/2.;
    mousePosDelta.second -= winH/2.;
    glfwSetCursorPos(window, winW/2., winH/2.);

    glfwPollEvents();
}

}
