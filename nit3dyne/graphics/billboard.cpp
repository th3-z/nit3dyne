#include "billboard.h"

namespace n3d {

// 1x1 quad
float vertices[] = {
    -.5f, .5f,
    -.5f, -.5f,
    .5f, -.5f,
    -.5f, .5f,
    .5f, -.5f,
    .5f, .5f,
};

Billboard::Billboard(std::pair<float, float> &&size, const std::shared_ptr<Texture> texture): texture(texture) {
    this->size = vec2(size.first, size.second);
    this->position = vec3(0.f, 0.f, 0.f);
    this->bind();
}

Billboard::~Billboard() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Billboard::draw(n3d::Shader &shader) {
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture->handle);

    glBindVertexArray(this->VAO);
    glDrawArrays(
        GL_TRIANGLES,
        0,
        6
    );

    glBindVertexArray(0);
}

void Billboard::bind() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            (void *) 0
    );

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

}