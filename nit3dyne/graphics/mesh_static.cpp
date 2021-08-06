#include "mesh_static.h"

namespace n3d {

MeshStatic::MeshStatic(const std::string &resourceName) : Mesh(resourceName, MeshType::STATIC) {
    this->bindModel();
}

void MeshStatic::bindModel() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    std::map<int, unsigned int> VBOs;

    this->bindMesh(this->gltf.meshes.front(), VBOs);

    // Cleanup
    glBindVertexArray(0);
    for (size_t i = 0; i < VBOs.size(); ++i) {
        glDeleteBuffers(1, &VBOs[i]);
    }
}

}