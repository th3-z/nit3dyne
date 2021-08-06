#include "camera.h"

namespace n3d {

Camera::Camera(const float fov, const std::pair<int, int> &viewPort) : fov(fov), viewPort(viewPort) {
    this->projection =
            perspective(radians(fov), (float) viewPort.first / viewPort.second, 0.1f, 10000.0f);
}

Camera::~Camera() = default;

mat4 Camera::getView() {
    this->front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
    this->front.y = sin(radians(this->pitch));
    this->front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));
    this->front = normalize(this->front);

    this->right = normalize(cross(front, this->up));

    return lookAt(this->position, this->position + this->front, this->up);
}

void Camera::setFov(float fov) {
    if (fov < 5.f)
        fov = 5.f;
    if (fov > 170.f)
        fov = 170.f;
    this->fov = fov;

    this->projection = perspective(
            radians(this->fov), (float) this->viewPort.first / this->viewPort.second, 0.1f, 10000.0f);
}

void Camera::update() {
    // Stub
}

}
