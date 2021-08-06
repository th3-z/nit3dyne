#ifndef GL_CAMERAFREE_H
#define GL_CAMERAFREE_H

#include "nit3dyne/core/math.h"

#include "nit3dyne/camera/camera.h"
#include "nit3dyne/core/input.h"

namespace n3d {

class CameraFree : public Camera {
public:
    CameraFree(const float fov, const std::pair<int, int> &viewPort);

    void update() override;
};

}

#endif // GL_CAMERAFREE_H
