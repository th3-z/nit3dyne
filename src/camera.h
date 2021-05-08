//
// Created by the_z on 03/05/2021.
//

#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum direction {
    UP = 1,
    LEFT = 2,
    DOWN = 4,
    RIGHT = 8,
    FORWARD = 16,
    BACKWARD = 32
};

class Camera {
public:
    glm::mat4 view;
    glm::mat4 projection;

    Camera();
    void move(int direction, int mX, int mY);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 wUp;

    float yaw = -90.f;
    float pitch = 0.f;

    const float speed = 0.001f;
    const float sensitivity = 1.5f;
};


#endif //GL_CAMERA_H
