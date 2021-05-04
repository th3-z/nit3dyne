//
// Created by the_z on 03/05/2021.
//

#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum key {
    UP = 0x001,
    LEFT = 0x002,
    DOWN = 0x004,
    RIGHT = 0x08
};

class Camera {
public:
    glm::mat4 view;
    glm::mat4 projection;

    Camera();
    void move(int k, int mX, int mY);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 wUp;

    float yaw = -90.f;
    float pitch = 0.f;

    const float speed = 0.1f;
    const float sensitivity = 1.5f;
};


#endif //GL_CAMERA_H
