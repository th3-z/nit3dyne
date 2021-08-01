#include "cameraFree.h"

namespace n3d {

CameraFree::CameraFree(const float fov, const std::pair<int, int> &viewPort) : Camera(fov, viewPort) {};

void CameraFree::update() {
    this->yaw += Input::mousePosDelta.first * (this->sensitivity * Display::timeDelta);
    this->pitch -= Input::mousePosDelta.second * (this->sensitivity * Display::timeDelta);

    if (Input::scrollDelta.second != 0.) {
        this->setFov(this->fov - Input::scrollDelta.second * 5);
    }

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;

    int direction = 0;
    if (Input::getKey(GLFW_KEY_W))
        direction |= Direction::FORWARD;
    if (Input::getKey(GLFW_KEY_A))
        direction |= Direction::LEFT;
    if (Input::getKey(GLFW_KEY_S))
        direction |= Direction::BACKWARD;
    if (Input::getKey(GLFW_KEY_D))
        direction |= Direction::RIGHT;
    if (Input::getKey(GLFW_KEY_SPACE))
        direction |= Direction::UP;
    if (Input::getKey(GLFW_KEY_LEFT_SHIFT))
        direction |= Direction::DOWN;

    if (Input::getKey(GLFW_KEY_LEFT_CONTROL))
        this->speed = 1500.f;
    else
        this->speed = 15.f;

    if (direction & Direction::FORWARD)
        this->position += this->speed * (float) Display::timeDelta * this->front;
    if (direction & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed * (float) Display::timeDelta;
    if (direction & Direction::BACKWARD)
        this->position -= this->speed * (float) Display::timeDelta * this->front;
    if (direction & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed * (float) Display::timeDelta;
    if (direction & Direction::UP)
        this->position += this->speed * (float) Display::timeDelta * this->up;
    if (direction & Direction::DOWN)
        this->position -= this->speed * (float) Display::timeDelta * this->up;
}

}
