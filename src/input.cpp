#include "input.h"

double Input::mouseLastX = 0.;
double Input::mouseLastY = 0.;

void Input::callbackMouse(GLFWwindow *window, double mouseX, double mouseY) {
    auto *windowState = (WindowState *) glfwGetWindowUserPointer(window);

    float xOffset = mouseX - mouseLastX;
    float yOffset = mouseY - mouseLastY;

    mouseLastX = mouseX;
    mouseLastY = mouseY;

    windowState->camera->handleMouse(xOffset, yOffset, (float) windowState->timeDelta);
}

void Input::callbackScroll(GLFWwindow *window, double xOffset, double yOffset) {
    auto *windowState = (WindowState *) glfwGetWindowUserPointer(window);

    windowState->camera->setFov(windowState->camera->fov - yOffset * 5);
}

void Input::callbackKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *windowState = (WindowState *) glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        // delete windowState->camera;
        windowState->camera =
            std::make_unique<CameraFps>(windowState->camera->fov, windowState->camera->viewPort);
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        windowState->camera =
            std::make_unique<CameraFree>(windowState->camera->fov, windowState->camera->viewPort);
    }
}

void Input::registerCallbacks(GLFWwindow *window) {
    glfwSetKeyCallback(window, callbackKey);
    glfwSetCursorPosCallback(window, callbackMouse);
    glfwSetScrollCallback(window, callbackScroll);
}

void Input::processContinuousInput(GLFWwindow *window) {
    auto *windowState = (WindowState *) glfwGetWindowUserPointer(window);

    int direction = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction |= Direction::FORWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction |= Direction::LEFT;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction |= Direction::BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction |= Direction::RIGHT;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        direction |= Direction::UP;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        direction |= Direction::DOWN;

    windowState->camera->handleDirection(direction, (float) windowState->timeDelta);
}
