#ifndef GL_MATERIAL_H
#define GL_MATERIAL_H

#include "nit3dyne/core/math.h"

namespace n3d {

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

namespace Materials {
    extern const Material basic;
    extern const Material metallic;
    extern const Material emissive;
} // namespace Materials

}

#endif // GL_MATERIAL_H
