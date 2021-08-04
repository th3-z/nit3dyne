#include "mesh_colored.h"


namespace n3d{


MeshColored::MeshColored(const std::string &resourceName) : Mesh(resourceName, MeshType::COLORED) {
    this->bindModel();
}

void MeshColored::bindModel() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    std::map<int, unsigned int> VBOs;
    glm::mat4 globalTransform(1.f);

    this->bindModelNodes(
            -1,
            this->gltf.scenes[this->gltf.defaultScene].nodes.front(),
            VBOs, globalTransform
    );

    // Cleanup
    glBindVertexArray(0);
    for (size_t i = 0; i < VBOs.size(); ++i) {
        glDeleteBuffers(1, &VBOs[i]);
    }
}

void MeshColored::bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, glm::mat4 &globalTransform) {
    tinygltf::Node node = this->gltf.nodes[nodeId];

    if (node.mesh >= 0)
        this->bindMesh(this->gltf.meshes[node.mesh], VBOs);

    for (int i : node.children)
        bindModelNodes(nodeId, i, VBOs, globalTransform);
}

}
