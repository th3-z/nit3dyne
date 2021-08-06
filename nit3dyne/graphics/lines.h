#ifndef GL_LINES_H
#define GL_LINES_H

#include "nit3dyne/core/math.h"
#include "nit3dyne/graphics/shader.h"

namespace n3d {

struct Line {
    vec3 vertexStart;
    vec3 colorStart;
    vec3 vertexEnd;
    vec3 colorEnd;
};


class Lines {
public:
    explicit Lines(std::vector<Line> &lines);

    ~Lines();

    void draw(Shader &shader, const mat4 &perspective, const mat4 &view);

private:
    void bind(std::vector<Line> &lines);

    unsigned int VAO;
    unsigned int count;

    mat4 modelMat = mat4(1.f);
};

}

#endif //GL_LINES_H
