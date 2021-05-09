//
// Created by the_z on 08/05/2021.
//

#ifndef GL_CAMERAFIXED_H
#define GL_CAMERAFIXED_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

class CameraFixed : public Camera {
public:
    CameraFixed();
    void handleDirection(int d, float timeDelta) override;
    void handleMouse(int x, int y) override;

private:
    const float fixedHeight = 10.f;
    const float fixedYaw = -90.f;
    const float fixedPitch = -45.f;
};


#endif //GL_CAMERAFIXED_H
