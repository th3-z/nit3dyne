#include "skybox.h"

namespace n3d {

const std::array<float, 108> vertices = {-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
                                         1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,

                                         -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
                                         -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

                                         1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,

                                         -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,

                                         -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
                                         1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,

                                         -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
                                         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

const std::array<std::string, 6> faceExts =
        {".posx.png", ".negx.png", ".posy.png", ".negy.png", ".posz.png", ".negz.png"};
const std::string path = "res/cubemap/";

Skybox::Skybox(const std::string &resourceName) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    std::vector<std::string> faceFilePaths(6);
    for (size_t i = 0; i < 6; ++i)
        faceFilePaths[i] = path + resourceName + faceExts[i];

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    assert(faceFilePaths.size() == 6);

    glGenTextures(1, &this->handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);

    int w, h, nChannels;
    unsigned char *data;

    stbi_set_flip_vertically_on_load(false);
    for (size_t i = 0; i < faceFilePaths.size(); ++i) {
        data = stbi_load(faceFilePaths[i].c_str(), &w, &h, &nChannels, 0);

        if (data) {
            //assert(nChannels == 3);
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
#ifndef NDEBUG
            std::cout << "Failed to load cubemap: " << faceFilePaths[i] << std::endl;
#endif
        }
        stbi_image_free(data);
    }
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(Shader &shader, glm::mat4 &&view, glm::mat4 &proj) {
    glDepthFunc(GL_LEQUAL);
    shader.use();

    shader.setUniform("view", view);
    shader.setUniform("proj", proj);

    glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &this->handle);
}

}