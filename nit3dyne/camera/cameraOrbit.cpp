#include "cameraOrbit.h"

namespace n3d {

CameraOrbit::CameraOrbit(const float fov, const std::pair<int, int> &viewPort) : Camera(fov, viewPort) {};

glm::mat4 CameraOrbit::getView() {
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -this->distance));
    view = glm::rotate(view, -this->pitch, glm::vec3(1.f, 0.f, 0.f));
    view = glm::rotate(view, -this->yaw, glm::vec3(0.f, 1.f, 0.f));
    view = glm::translate(view, this->target);
    return view;
}

void CameraOrbit::update() {
    this->yaw += this->sensitivity * Display::timeDelta * Input::mousePosDelta.first;
    this->pitch -= this->sensitivity * Display::timeDelta * Input::mousePosDelta.second;

    if (this->pitch > 89.0f)
        this->pitch = 89.0;
    if (this->pitch < -89.0f)
        this->pitch = -89.0;

    if (Input::scrollDelta.second != 0.) {
        this->setFov(this->fov - Input::scrollDelta.second * 5);
    }
}

}
