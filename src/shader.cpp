//
// Created by the_z on 24/04/2021.
//

#include "shader.h"

Shader::Shader(const char *vPath, const char *fPath) {
    std::string vSrc;
    std::string fSrc;
    std::ifstream vFile;
    std::ifstream fFile;

    vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vFile.open(vPath);
        fFile.open(fPath);
        std::stringstream vStream, fStream;

        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        vSrc = vStream.str();
        fSrc = fStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "Shader read error" << std::endl;
    }

    unsigned int vId, fId;
    int success;
    char infoLog[512];

    vId = glCreateShader(GL_VERTEX_SHADER);
    const char *vSrcC = vSrc.c_str();
    glShaderSource(vId, 1, &vSrcC, NULL);
    glCompileShader(vId);
    glGetShaderiv(vId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vId, 512, NULL, infoLog);
        std::cout << "Error: Failed to compile vertex shader\n" << infoLog << std::endl;
    }

    fId = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fSrcC = fSrc.c_str();
    glShaderSource(fId, 1, &fSrcC, NULL);
    glCompileShader(fId);
    glGetShaderiv(fId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vId, 512, NULL, infoLog);
        std::cout << "Error: Failed to compile fragment shader\n" << infoLog << std::endl;
    }

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vId);
    glAttachShader(this->handle, fId);
    glLinkProgram(this->handle);

    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->handle, 512, NULL, infoLog);
        std::cout << "Error: Failed to link shader program\n" << infoLog << std::endl;
    }

    glDeleteShader(vId);
    glDeleteShader(fId);
}

void Shader::use() {
    glUseProgram(this->handle);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(
        glGetUniformLocation(this->handle, name.c_str()),
        (int)value
    );
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(
        glGetUniformLocation(this->handle, name.c_str()),
        value
    );
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(
        glGetUniformLocation(this->handle, name.c_str()),
        value
    );
}

void Shader::setMat4(const std::string &name, glm::mat4 &mat) const {
    glUniformMatrix4fv(
            glGetUniformLocation(this->handle, name.c_str()),
            1,  // Send one
            GL_FALSE,  // Don't transpose (swap rows/cols)
            glm::value_ptr(mat)
    );
}

void Shader::setVec3(const std::string &name, glm::vec3 &vec) const {
    glUniform3fv(
            glGetUniformLocation(this->handle, name.c_str()),
            1,  // Send one
            glm::value_ptr(vec)
    );
}

Shader::~Shader() {
    glDeleteProgram(this->handle);
}
