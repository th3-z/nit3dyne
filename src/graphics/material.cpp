//
// Created by the_z on 18/05/2021.
//

#include "material.h"

namespace Materials {
    const Material basic = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            4.0f
    };

    const Material metallic = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.7f, 0.7f, 0.8f),
            32.0f
    };

    const Material emissive = {
            glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec3(3.0f, 3.0f, 3.0f),
            glm::vec3(0.4, 0.4, 0.4),
            24.f
    };
}