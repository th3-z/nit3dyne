#include "shader.h"

namespace n3d {

Shader::Shader(const char *vPath, const char *fPath) : Shader(vPath, fPath, nullptr) {}

Shader::Shader(const char *vPath, const char *fPath, const char *gPath) {
    std::string vSrc;
    std::string fSrc;
    std::string gSrc;
    std::ifstream vFile;
    std::ifstream fFile;
    std::ifstream gFile;

    // FIXME: duplicates not needed
    vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vStream, fStream, gStream;

        vFile.open(vPath);
        fFile.open(fPath);

        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        vSrc = vStream.str();
        fSrc = fStream.str();

        // TODO: refactor if statements that check if there is shader type
        if (gPath != nullptr) {
            gFile.open(gPath);
            gStream << gFile.rdbuf();
            gSrc = gStream.str();
        }
    } catch (std::ifstream::failure &e) { std::cout << "Shader read error" << std::endl; }

    unsigned int vId, fId, gId;
    int success;
    char infoLog[512];

    vId = glCreateShader(GL_VERTEX_SHADER);
    const char *vSrcC = vSrc.c_str();
    glShaderSource(vId, 1, &vSrcC, NULL);
    glCompileShader(vId);
    glGetShaderiv(vId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vId, 512, NULL, infoLog);
        std::cout << "Error: Failed to compile vertex shader\n" << infoLog << std::endl;
    }

    fId = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fSrcC = fSrc.c_str();
    glShaderSource(fId, 1, &fSrcC, NULL);
    glCompileShader(fId);
    glGetShaderiv(fId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fId, 512, NULL, infoLog);
        std::cout << "Error: Failed to compile fragment shader\n" << infoLog << std::endl;
    }

    if (gPath != nullptr) {
        gId = glCreateShader(GL_GEOMETRY_SHADER);
        const char *gSrcC = gSrc.c_str();
        glShaderSource(gId, 1, &gSrcC, NULL);
        glCompileShader(gId);
        glGetShaderiv(gId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(gId, 512, NULL, infoLog);
            std::cout << "Error: Failed to compile geometry shader\n" << infoLog << std::endl;
        }
    }

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vId);
    glAttachShader(this->handle, fId);
    if (gPath != nullptr)
        glAttachShader(this->handle, gId);
    glLinkProgram(this->handle);

    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->handle, 512, NULL, infoLog);
        std::cout << "Error: Failed to link shader program\n" << infoLog << std::endl;
        std::cout << "VS: " << vPath << std::endl;
        std::cout << "FS: " << fPath << std::endl;
        if (gPath != nullptr) {
            std::cout << "GS: " << gPath << std::endl;
        }
    }

    glDeleteShader(vId);
    glDeleteShader(fId);
    if (gPath != nullptr) {
        glDeleteShader(gId);
    }
}

void Shader::use() const {
    glUseProgram(this->handle);
}

void Shader::setUniform(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.c_str()), (int) value);
}

void Shader::setUniform(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(this->handle, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->handle, name.c_str()),
                       1,        // Send one
                       GL_FALSE, // Don't transpose (swap rows/cols)
                       glm::value_ptr(mat));
}

void Shader::setUniform(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.c_str()),
                       1,        // Send one
                       GL_FALSE, // Don't transpose (swap rows/cols)
                       glm::value_ptr(mat));
}

void Shader::setUniform(const std::string &name, const glm::vec3 &vec) const {
    glUniform3fv(glGetUniformLocation(this->handle, name.c_str()),
                 1, // Send one
                 glm::value_ptr(vec));
}

void Shader::setUniform(const std::string &name, const glm::vec4 &vec) const {
    glUniform4fv(glGetUniformLocation(this->handle, name.c_str()),
                 1, // Send one
                 glm::value_ptr(vec));
}

void Shader::setUniform(const std::string &name, const std::vector<glm::mat4> &mats) const {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.c_str()),
                       mats.size(), // Send all
                       GL_FALSE,
                       glm::value_ptr(mats.front()));
}

Shader::~Shader() {
    glDeleteProgram(this->handle);
}

void Shader::attachMaterial(const Material &material) const {
    this->setUniform("material.ambient", material.ambient);
    this->setUniform("material.diffuse", material.diffuse);
    this->setUniform("material.specular", material.specular);
    this->setUniform("material.shininess", material.shininess);
}

void Shader::setDirectionalLight(const DirectionalLight &dLight) const {
    this->setUniform("dLight.direction", dLight.direction);

    this->setUniform("dLight.ambient", dLight.ambient);
    this->setUniform("dLight.diffuse", dLight.diffuse);
    this->setUniform("dLight.specular", dLight.specular);
}

void Shader::setSpotLight(const SpotLight &sLight) const {
    this->setUniform("sLight.direction", sLight.direction);
    this->setUniform("sLight.position", sLight.position);
    this->setUniform("sLight.cutOff", sLight.cutOff);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &vec) const {
    glUniform2fv(glGetUniformLocation(this->handle, name.c_str()),
                 1, // Send one
                 glm::value_ptr(vec));
}

}