#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Camera {
public:
    Camera(float fov, const std::pair<int, int> &viewPort);
    virtual ~Camera();

    virtual glm::mat4 getView();
    virtual void update();

    void setFov(float fov);

    std::pair<int, int> viewPort;
    glm::mat4 projection;
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    float fov;

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
