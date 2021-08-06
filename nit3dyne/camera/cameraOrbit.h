#ifndef GL_CAMERAORBIT_H
#define GL_CAMERAORBIT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "nit3dyne/camera/camera.h"
#include "nit3dyne/core/input.h"
#include "nit3dyne/core/display.h"

namespace n3d {

class CameraOrbit : public Camera {
public:
    CameraOrbit(float fov, const std::pair<int, int> &viewPort);
    ~CameraOrbit() override = default;

    glm::mat4 getView() override;
    void update() override;

private:
    glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);
    float distance = 10.f;
};

}

#endif // GL_CAMERAORBIT_H
