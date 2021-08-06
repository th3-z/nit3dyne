#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <memory>

#include "nit3dyne/graphics/mesh.h"
#include "nit3dyne/graphics/mesh_static.h"
#include "nit3dyne/graphics/mesh_animated.h"
#include "nit3dyne/graphics/mesh_colored.h"
#include "nit3dyne/graphics/shader.h"
#include "nit3dyne/graphics/texture.h"
#include <glm/glm.hpp>

namespace n3d {

class Model {
public:
    // TODO Add another constructor with a default material
    explicit Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

    ~Model();

    void draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view);

    void setMaterial(const Material &material);

    void translate(float x, float y, float z);

    void scale(float x, float y, float z);

    void rotate(float deg, float x, float y, float z, bool normalize = true);

    glm::mat4 modelMat;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    const Material *material = &Materials::basic;
};

}

#endif // GL_MODEL_H
