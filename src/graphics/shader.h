//
// Created by the_z on 24/04/2021.
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "material.h"
#include "lighting.h"


class Shader {

public:
    unsigned int handle;

    Shader(const char *vPath, const char *fPath);
    ~Shader();
    void use() const;

    void attachMaterial(const Material &material) const;
    void setDirectionalLight(const DirectionalLight &dLight) const;
    void setSpotLight(const SpotLight &sLight) const;

    void setUniform(const std::string &name, const bool value) const;
    void setUniform(const std::string &name, const int value) const;
    void setUniform(const std::string &name, const float value) const;
    void setUniform(const std::string &name, const glm::mat3 &mat) const;
    void setUniform(const std::string &name, const glm::mat4 &mat) const;
    void setUniform(const std::string &name, const glm::vec3 &vec) const;
    void setUniform(const std::string &name, const glm::vec4 &vec) const;
};


#endif //GL_SHADER_H
