#include "lines.h"

namespace n3d {

Lines::Lines(std::vector<Line> &lines) {
    this->bind(lines);
}

Lines::~Lines() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Lines::bind(std::vector<Line> &lines) {
    this->count = lines.size() * 2;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Line) * lines.size(),
            &lines[0],
            GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Line) / 2,
            (void *) 0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1, 3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Line) / 2,
            (void *) offsetof(Line, colorStart)
    );

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

void Lines::draw(Shader &shader, const mat4 &perspective, const mat4 &view) {
    shader.use();
    mat4 mvp = perspective * view * this->modelMat;
    shader.setUniform("mvp", mvp);

    glBindVertexArray(this->VAO);
    glDrawArrays(
            GL_LINES,
            0,
            this->count
    );

    glBindVertexArray(0);
}

}
