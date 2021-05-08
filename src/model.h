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

#include "tiny_gltf.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

void dbgModel(tinygltf::Model &model);

class Model {
public:
    Model(const char* filePath);
    ~Model();
    void render(Shader &shader);

    glm::mat4 modelMat = glm::mat4(1.f);

private:
    void bindModel();
    void bindModelNodes(tinygltf::Node &node);
    void bindMesh(tinygltf::Mesh &mesh);

    void drawModelNodes(tinygltf::Node &node);
    void drawMesh(tinygltf::Mesh &mesh);


    tinygltf::Model model;
    unsigned int VAO;//, VBO, EBO;
    std::map<int, unsigned int> VBOs;
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Texture> textures;
    glm::vec3 color = glm::vec3(.5f, .5f, .5f);
};


#endif //GL_MODEL_H
