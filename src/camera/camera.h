//
// Created by the_z on 03/05/2021.
//

#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction {
    UP = 1,
    LEFT = 2,
    DOWN = 4,
    RIGHT = 8,
    FORWARD = 16,
    BACKWARD = 32
};


class Camera {
public:
    Camera();
    virtual ~Camera();

    virtual void handleDirection(int d, float timeDelta) = 0;
    virtual void handleMouse(int x, int y) = 0;
    glm::mat4 getView();

    glm::vec3 position;

protected:
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw = -90.f;
    float pitch = -0.f;

    const float speed = 5.f;  // units per second
    const float sensitivity = 1.5f;
};


#endif //GL_CAMERA_H
