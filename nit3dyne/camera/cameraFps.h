#ifndef GL_CAMERAFPS_H
#define GL_CAMERAFPS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "../input.h"
#include "../display.h"

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
