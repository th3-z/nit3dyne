//
// Created by the_z on 08/05/2021.
//

#include "cameraFixed.h"

CameraFixed::CameraFixed() {
    this->position = glm::vec3(0.f, this->fixedHeight, 10.f);
    this->pitch = this->fixedPitch;
    this->yaw = this->fixedYaw;
}

void CameraFixed::handleDirection(int d) {
    if (d & Direction::FORWARD) {
        this->position.x += this->speed * this->front.x;
        this->position.z += this->speed * this->front.z;
    }
    if (d & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed;
    if (d & Direction::BACKWARD) {
        this->position.x -= this->speed * this->front.x;
        this->position.z -= this->speed * this->front.z;
    }
    if (d & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed;
}

void CameraFixed::handleMouse(int x, int y) {
    this->yaw += x * this->sensitivity;
}