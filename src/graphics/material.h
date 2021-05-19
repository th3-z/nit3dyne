//
// Created by the_z on 18/05/2021.
//

#ifndef GL_MATERIAL_H
#define GL_MATERIAL_H


#include <glm/vec3.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

namespace Materials {
    extern const Material basic;
    extern const Material metallic;
    extern const Material emissive;
}




#endif //GL_MATERIAL_H
