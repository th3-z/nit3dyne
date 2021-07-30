#ifndef GL_CAMERAFREE_H
#define GL_CAMERAFREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "../input.h"

namespace n3d {

class CameraFree : public Camera {
public:
    CameraFree(const float fov, const std::pair<int, int> &viewPort);

    void update() override;
};

}

#endif // GL_CAMERAFREE_H
