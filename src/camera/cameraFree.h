//
// Created by the_z on 08/05/2021.
//

#ifndef GL_CAMERAFREE_H
#define GL_CAMERAFREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

class CameraFree : public Camera {
public:
    CameraFree();
    void handleDirection(int d, float timeDelta) override;
    void handleMouse(int x, int y) override;
};


#endif //GL_CAMERAFREE_H
