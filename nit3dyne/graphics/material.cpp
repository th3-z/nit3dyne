#include "material.h"

namespace n3d::Materials {
const Material basic = {vec3(1.0f, 1.0f, 1.0f),
                        vec3(1.0f, 1.0f, 1.0f),
                        vec3(0.5f, 0.5f, 0.5f),
                        4.0f};

const Material metallic = {vec3(1.0f, 1.0f, 1.0f),
                           vec3(1.0f, 1.0f, 1.0f),
                           vec3(0.7f, 0.7f, 0.8f),
                           32.0f};

const Material emissive = {vec3(10.0f, 10.0f, 10.0f),
                           vec3(3.0f, 3.0f, 3.0f),
                           vec3(0.4, 0.4, 0.4),
                           24.f};
}