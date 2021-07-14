#include "cameraFps.h"

CameraFps::CameraFps(const float fov, const std::pair<int, int> &viewPort)
: Camera(fov, viewPort) {
    this->position = glm::vec3(0.f, this->playerHeight, 10.f);
}

void CameraFps::update() {
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

    if (direction & Direction::FORWARD) {
        this->position.x += this->speed * (float) Display::timeDelta * this->front.x;
        this->position.z += this->speed * (float) Display::timeDelta * this->front.z;
    }
    if (direction & Direction::LEFT) {
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed * (float) Display::timeDelta;
    }
    if (direction & Direction::BACKWARD) {
        this->position.x -= this->speed * (float) Display::timeDelta * this->front.x;
        this->position.z -= this->speed * (float) Display::timeDelta * this->front.z;
    }
    if (direction & Direction::RIGHT) {
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed * (float) Display::timeDelta;
    }

    this->yaw += Input::mousePosDelta.first * (this->sensitivity * Display::timeDelta);
    this->pitch -= Input::mousePosDelta.second * (this->sensitivity * Display::timeDelta);

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;
}
