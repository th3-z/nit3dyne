#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction { UP = 1, LEFT = 2, DOWN = 4, RIGHT = 8, FORWARD = 16, BACKWARD = 32 };

class Camera {
public:
    Camera(const float fov, const std::pair<int, int> &viewPort);
    virtual ~Camera();

    virtual void handleDirection(int d, float timeDelta) = 0;
    virtual void handleMouse(int x, int y, float timeDelta) = 0;
    void setFov(float fov);
    glm::mat4 getView();

    glm::mat4 projection;
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    float fov;
    std::pair<int, int> viewPort;

protected:
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 right;

    float yaw = -90.f;
    float pitch = -0.f;

    const float speed = 5.f; // units per second
    const float sensitivity = 5.f;
};

#endif // GL_CAMERA_H
