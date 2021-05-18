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


class Shader {

public:
    unsigned int handle;

    Shader(const char *vPath, const char *fPath);
    ~Shader();
    void use() const;

    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, glm::mat3 &mat) const;
    void setUniform(const std::string &name, glm::mat4 &mat) const;
    void setUniform(const std::string &name, glm::vec3 &vec) const;
};


#endif //GL_SHADER_H
