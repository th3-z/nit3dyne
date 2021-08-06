#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "nit3dyne/core/math.h"

namespace n3d {

class Camera {
public:
    Camera(float fov, const std::pair<int, int> &viewPort);

    virtual ~Camera();

    virtual mat4 getView();
    virtual void update();
    void setFov(float fov);

    std::pair<int, int> viewPort;
    mat4 projection;
    vec3 position = vec3(0.f, 0.f, 0.f);
    float fov;

protected:
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.f, 1.f, 0.f);
    vec3 right;

    float yaw = -90.f;
    float pitch = -0.f;

    float speed = 15.f; // units per second
    float sensitivity = .08f;
};

}

#endif // GL_CAMERA_H
