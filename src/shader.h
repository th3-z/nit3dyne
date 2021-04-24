//
// Created by the_z on 24/04/2021.
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {

public:
    unsigned int id;

    Shader(const char *vPath, const char *fPath);
    void use();

    // Uniform setters
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};


#endif //GL_SHADER_H
