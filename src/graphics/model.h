#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <memory>

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

class Model {
public:
    // TODO Add another constructor with a default material
    explicit Model(const std::shared_ptr<Mesh> mesh, const std::shared_ptr<Texture> texture);
    ~Model();

    void draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view);
    void setMaterial(const Material &material);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float deg, float x, float y, float z, bool normalize = true);

    glm::mat4 modelMat;

private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    const Material *material = &Materials::basic;
};

#endif // GL_MODEL_H
