#ifndef GL_CAMERAFPS_H
#define GL_CAMERAFPS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

class CameraFps : public Camera {
public:
    CameraFps();
    void handleDirection(int d, float timeDelta) override;
    void handleMouse(int x, int y, float timeDelta) override;

private:
    const float playerHeight = 2.f;
};

#endif // GL_CAMERAFPS_H
