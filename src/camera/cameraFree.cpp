//
// Created by the_z on 08/05/2021.
//

#include "cameraFree.h"

CameraFree::CameraFree() = default;

void CameraFree::handleMouse(int x, int y) {
    this->yaw += x * this->sensitivity;
    this->pitch -= y * this->sensitivity;

    if (this->pitch > 89.0f) this->pitch = 89.0;
    if (this->pitch < -89.0f) this->pitch = -89.0;
}

void CameraFree::handleDirection(int d) {
    if (d & Direction::FORWARD)
        this->position += this->speed * this->front;
    if (d & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed;
    if (d & Direction::BACKWARD)
        this->position -= this->speed * this->front;
    if (d & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed;
    if (d & Direction::UP)
        this->position += this->speed * this->worldUp;
    if (d & Direction::DOWN)
        this->position -= this->speed * this->worldUp;
}
