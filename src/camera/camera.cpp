#include "camera.h"

Camera::Camera(const float fov, const std::pair<int, int> &viewPort): fov(fov), viewPort(viewPort) {
    this->projection =
        glm::perspective(glm::radians(fov), (float) viewPort.first / viewPort.second, 0.1f, 100.0f);
}

Camera::~Camera() = default;

glm::mat4 Camera::getView() {
    this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(this->front);

    this->right = glm::normalize(glm::cross(front, this->up));
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::setFov(float fov) {
    if (fov < 5.f)
        fov = 5.f;
    if (fov > 170.f)
        fov = 170.f;
    this->fov = fov;

    this->projection = glm::perspective(
        glm::radians(this->fov), (float) this->viewPort.first / this->viewPort.second, 0.1f, 100.0f);
}
