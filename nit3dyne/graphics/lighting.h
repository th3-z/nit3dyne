#ifndef GL_LIGHTING_H
#define GL_LIGHTING_H

#include "nit3dyne/core/math.h"

namespace n3d {

struct DirectionalLight {
    vec4 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    DirectionalLight() :
        direction(vec4(-0.f, -1.0f, -0.5f, 0.0f)),
        ambient(vec3(0.2f, 0.2f, 0.2f)),
        diffuse(vec3(0.7f, 0.7f, 0.7f)), specular(vec3(0.5f, 0.5f, 0.5f)) {};
};

struct PointLight {
    vec4 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight() :
        position(vec4(1.0f, 1.0f, 1.0f, 1.0f)), ambient(vec3(0.1f, 0.1f, 0.1f)),
        diffuse(vec3(0.7f, 0.75, 0.7f)), specular(vec3(0.5f, 0.5f, 0.5f)), constant(1.f), linear(1.f),
        quadratic(1.f) {};
};

struct SpotLight {
    vec4 position;
    vec4 direction;

    float cutOff;

    SpotLight() :
        // Defaults are in view-space, it will never move relative to the view
        position(vec4(0.5f, 0.5f, 0.f, 0.0f)), direction(vec4(0.0f, 0.0f, -1.0f, 0.0f)),
        cutOff(cos(radians(24.5f))) {};
};

}

#endif // GL_LIGHTING_H
