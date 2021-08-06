#ifndef NIT3DYNE_EX_BILLBOARD_H
#define NIT3DYNE_EX_BILLBOARD_H

#include <memory>
#include <glad/glad.h>

#include "nit3dyne/core/math.h"
#include "nit3dyne/graphics/shader.h"
#include "nit3dyne/graphics/texture.h"


namespace n3d {


class Billboard {
public:
    Billboard(std::pair<float, float> &&size, const std::shared_ptr<Texture> texture);
    ~Billboard();

    void draw(Shader &shader);

    vec2 size;
    vec3 position;
    bool viewScale = true;

private:
    void bind();

    unsigned int VAO;
    std::shared_ptr<Texture> texture;


    mat4 modelMat;
};

}
#endif //NIT3DYNE_EX_BILLBOARD_H
