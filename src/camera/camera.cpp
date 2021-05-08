//
// Created by the_z on 03/05/2021.
//


#include "camera.h"


Camera::Camera():
position(glm::vec3(0.f, 0.f, 0.f)),
up(glm::vec3(0.0f, 1.0f,  0.0f)),
front(glm::vec3(0.0f, 0.0f, -1.0f)),
worldUp(glm::vec3(0.f, 1.f, 0.f)) {

}

Camera::~Camera() {
    std::cout << "Destroyed Camera" << std::endl;
}

glm::mat4 Camera::getView() {
    this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(this->front);

    this->right = glm::normalize(glm::cross(front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    return glm::lookAt(this->position, this->position + this->front, this->up);
}

