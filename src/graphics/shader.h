#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lighting.h"
#include "material.h"

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

#endif // GL_SHADER_H
