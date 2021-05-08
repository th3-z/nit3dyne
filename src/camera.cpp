//
// Created by the_z on 03/05/2021.
//


#include "camera.h"

Camera::Camera():
position(glm::vec3(0.f, 10.f, 10.f)),
up(glm::vec3(0.0f, 1.0f,  0.0f)),
front(glm::vec3(0.0f, 0.0f, -1.0f)),
wUp(glm::vec3(0.f, 1.f, 0.f)),
projection(glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)) {

}

void Camera::move(int direction, int mX, int mY) {
    if (direction & Direction::FORWARD) {
        if (this->mode == Mode::FREECAM) {
            this->position += this->speed * this->front;
        } else {
            this->position.x += this->speed * this->front.x;
            this->position.z += this->speed * this->front.z;
        }
    }
    if (direction & Direction::LEFT)
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed;
    if (direction & Direction::BACKWARD) {
        if (this->mode == Mode::FREECAM) {
            this->position -= this->speed * this->front;
        } else {
            this->position.x -= this->speed * this->front.x;
            this->position.z -= this->speed * this->front.z;
        }
    }
    if (direction & Direction::RIGHT)
        this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed;
    if (direction & Direction::UP && this->mode == Mode::FREECAM)
        this->position += this->speed * this->wUp;
    if (direction & Direction::DOWN && this->mode == Mode::FREECAM)
        this->position -= this->speed * this->wUp;

    this->yaw += mX * this->sensitivity;
    if (this->mode == Mode::FREECAM) {

        this->pitch -= mY * this->sensitivity;

        if (this->pitch > 89.0f) this->pitch = 89.0;
        if (this->pitch < -89.0f) this->pitch = -89.0;
    }

    this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(this->front);

    this->right = glm::normalize(glm::cross(front, this->wUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    this->view = glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::setMode(Mode mode) {
    if (mode == Mode::FIXED) {
        this->position.z = this->fixedHeight;
        this->pitch = fixedPitch;
    }
    this->mode = mode;
}
