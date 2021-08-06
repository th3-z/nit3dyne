#ifndef GL_CAMERAFREE_H
#define GL_CAMERAFREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
