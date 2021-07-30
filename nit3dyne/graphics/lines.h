#ifndef GL_LINES_H
#define GL_LINES_H

#include <glm/glm.hpp>
#include "shader.h"

namespace n3d {

struct Line {
    glm::vec3 vertexStart;
    glm::vec3 colorStart;
    glm::vec3 vertexEnd;
    glm::vec3 colorEnd;
};


class Lines {
public:
    explicit Lines(std::vector<Line> &lines);

    ~Lines();

    void draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view);

private:
    void bind(std::vector<Line> &lines);

    unsigned int VAO;
    unsigned int count;

    glm::mat4 modelMat = glm::mat4(1.f);
};

}

#endif //GL_LINES_H
