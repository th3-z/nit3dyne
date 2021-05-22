#ifndef GL_LIGHTING_H
#define GL_LIGHTING_H

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec4 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight():
    direction(glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f)), ambient(glm::vec3(0.06f, 0.06f, 0.06f)),
    diffuse(glm::vec3(0.7f, 0.7f, 0.7f)), specular(glm::vec3(0.5f, 0.5f, 0.5f)){};
};

struct PointLight {
    glm::vec4 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight():
    position(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), ambient(glm::vec3(0.1f, 0.1f, 0.1f)),
    diffuse(glm::vec3(0.7f, 0.75, 0.7f)), specular(glm::vec3(0.5f, 0.5f, 0.5f)), constant(1.f), linear(1.f),
    quadratic(1.f){};
};

struct SpotLight {
    glm::vec4 position;
    glm::vec4 direction;

    float cutOff;

    SpotLight():
    // Defaults are in view-space, it will never move relative to the view
    position(glm::vec4(0.5f, 0.5f, 0.f, 0.0f)), direction(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)),
    cutOff(glm::cos(glm::radians(24.5f))){};
};

#endif // GL_LIGHTING_H
