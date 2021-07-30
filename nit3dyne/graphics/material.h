#ifndef GL_MATERIAL_H
#define GL_MATERIAL_H

#include <glm/vec3.hpp>

namespace n3d {

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
} // namespace Materials

}

#endif // GL_MATERIAL_H
