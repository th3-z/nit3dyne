#ifndef GL_CAMERAFPS_H
#define GL_CAMERAFPS_H

#include "nit3dyne/core/math.h"
#include "nit3dyne/camera/camera.h"
#include "nit3dyne/core/input.h"
#include "nit3dyne/core/display.h"

namespace n3d {

class CameraFps : public Camera {
public:
    CameraFps(const float fov, const std::pair<int, int> &viewPort);

    void update() override;

private:
    const float playerHeight = 1.7f;
};

}

#endif // GL_CAMERAFPS_H
