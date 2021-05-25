#include "cameraFree.h"

CameraFree::CameraFree(const float fov, const std::pair<int, int> &viewPort): Camera(fov, viewPort){};

void CameraFree::handleMouse(int x, int y, float timeDelta) {
    this->yaw += x * (this->sensitivity * timeDelta);
    this->pitch -= y * (this->sensitivity * timeDelta);

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;
}

void CameraFree::handleDirection(int d, float timeDelta) {
    if (d & Direction::FORWARD)
        this->position += this->speed * timeDelta * this->front;
    if (d & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed * timeDelta;
    if (d & Direction::BACKWARD)
        this->position -= this->speed * timeDelta * this->front;
    if (d & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed * timeDelta;
    if (d & Direction::UP)
        this->position += this->speed * timeDelta * this->up;
    if (d & Direction::DOWN)
        this->position -= this->speed * timeDelta * this->up;
}
