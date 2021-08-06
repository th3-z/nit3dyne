#ifndef GL_CAMERAORBIT_H
#define GL_CAMERAORBIT_H

#include "nit3dyne/core/math.h"
#include "nit3dyne/camera/camera.h"
#include "nit3dyne/core/input.h"
#include "nit3dyne/core/display.h"

namespace n3d {

class CameraOrbit : public Camera {
public:
    CameraOrbit(float fov, const std::pair<int, int> &viewPort);
    ~CameraOrbit() override = default;

    mat4 getView() override;
    void update() override;

private:
    vec3 target = vec3(0.f, 0.f, 0.f);
    float distance = 10.f;
};

}

#endif // GL_CAMERAORBIT_H
