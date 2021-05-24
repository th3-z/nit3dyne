#ifndef GL_MESH_H
#define GL_MESH_H

#include <glad/glad.h>
#ifndef NDEBUG
#include <iostream>
#endif
#include <map>
#include <string>

#include "tiny_gltf.h"

class Mesh {
public:
    explicit Mesh(const std::string &resourceName);
    ~Mesh();

    void draw();

private:
    unsigned int VAO;
    tinygltf::Model gltf;

    void bindBuffers();
};

#endif // GL_MESH_H
