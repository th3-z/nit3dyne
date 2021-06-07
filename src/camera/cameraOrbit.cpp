#include "cameraOrbit.h"

CameraOrbit::CameraOrbit(const float fov, const std::pair<int, int> &viewPort): Camera(fov, viewPort){};

void CameraOrbit::handleMouse(int x, int y, float timeDelta) {
    this->yaw += x * (this->sensitivity * timeDelta);
    this->pitch -= y * (this->sensitivity * timeDelta);

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;
}

void CameraOrbit::handleDirection(int d, float timeDelta) {
    // Stub
}

glm::mat4 CameraOrbit::getView() {
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -this->distance));
    view = glm::rotate(view, -this->pitch, glm::vec3(1.f, 0.f, 0.f));
    view = glm::rotate(view, -this->yaw, glm::vec3(0.f, 1.f, 0.f));
    view = glm::translate(view, this->target);
    return view;
}
