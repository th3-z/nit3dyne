//
// Created by the_z on 05/05/2021.
//

#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "texture.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Model {
public:
    Model(std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Texture> &textures);
    ~Model();
    void render(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Texture> textures;
    glm::vec3 color = glm::vec3(.5f, .5f, .5f);
};


#endif //GL_MODEL_H
