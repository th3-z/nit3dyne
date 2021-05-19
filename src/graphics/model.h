//
// Created by the_z on 18/05/2021.
//

#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Model {
public:
    // TODO Add another constructor with a default material
    explicit Model(const std::string &meshFilename, const std::string &textureFilename);
    ~Model();

    void draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view);
    void setMaterial(const Material &material);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float deg, float x, float y, float z, bool normalize = true);

    glm::mat4 modelMat;
private:
    Mesh mesh;
    Texture texture;
    const Material *material = &Materials::basic;
};


#endif //GL_MODEL_H
