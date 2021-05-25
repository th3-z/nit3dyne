#include "cameraFps.h"

CameraFps::CameraFps(const float fov, const std::pair<int, int> &viewPort): Camera(fov, viewPort) {
    this->position = glm::vec3(0.f, this->playerHeight, 10.f);
}

void CameraFps::handleDirection(int d, float timeDelta) {
    if (d & Direction::FORWARD) {
        this->position.x += this->speed * timeDelta * this->front.x;
        this->position.z += this->speed * timeDelta * this->front.z;
    }
    if (d & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed * timeDelta;
    if (d & Direction::BACKWARD) {
        this->position.x -= this->speed * timeDelta * this->front.x;
        this->position.z -= this->speed * timeDelta * this->front.z;
    }
    if (d & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed * timeDelta;
}

void CameraFps::handleMouse(int x, int y, float timeDelta) {
    this->yaw += x * (this->sensitivity * timeDelta);
    this->pitch -= y * (this->sensitivity * timeDelta);

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;
}
