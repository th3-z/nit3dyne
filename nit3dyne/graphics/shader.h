#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glad/glad.h>


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "nit3dyne/core/math.h"
#include "nit3dyne/graphics/lighting.h"
#include "nit3dyne/graphics/material.h"
#include "nit3dyne/graphics/shader_preprocess.h"

namespace n3d {

class Shader {

public:
    unsigned int handle;

    Shader(const char *vPath, const char *fPath);

    Shader(const char *vPath, const char *fPath, const char *gPath);

    ~Shader();

    void use() const;

    void attachMaterial(const Material &material) const;

    void setDirectionalLight(const DirectionalLight &dLight) const;

    void setSpotLight(const SpotLight &sLight) const;

    void setUniform(const std::string &name, const bool value) const;

    void setUniform(const std::string &name, const int value) const;

    void setUniform(const std::string &name, const float value) const;

    void setUniform(const std::string &name, const mat3 &mat) const;

    void setUniform(const std::string &name, const mat4 &mat) const;

    void setUniform(const std::string &name, const vec2 &vec) const;

    void setUniform(const std::string &name, const vec3 &vec) const;

    void setUniform(const std::string &name, const vec4 &vec) const;

    void setUniform(const std::string &name, const std::vector<mat4> &mats) const;
};

}

#endif // GL_SHADER_H
