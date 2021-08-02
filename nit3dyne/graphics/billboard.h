#ifndef NIT3DYNE_EX_BILLBOARD_H
#define NIT3DYNE_EX_BILLBOARD_H

#include <memory>
#include <glad/glad.h>

#include "glm/glm.hpp"
#include "shader.h"
#include "texture.h"


namespace n3d {


class Billboard {
public:
    Billboard(std::pair<float, float> &&size, const std::shared_ptr<Texture> texture);
    ~Billboard();

    void draw(Shader &shader);

    glm::vec2 size;
    glm::vec3 position;

private:
    void bind();

    unsigned int VAO;
    std::shared_ptr<Texture> texture;


    glm::mat4 modelMat;
};

}
#endif //NIT3DYNE_EX_BILLBOARD_H
