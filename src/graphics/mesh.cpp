#include "mesh.h"

std::string ext = ".glb";
std::string path = "res/mesh/";



MeshIf::MeshIf(const std::string &resourceName, MeshType meshType):
meshType(meshType) {
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool res = loader.LoadBinaryFromFile(&this->gltf, &err, &warn, path + resourceName + ext);
    if (!warn.empty())
        std::cout << "Mesh warning: " << warn << std::endl;
    if (!err.empty())
        std::cout << "Mesh error: " << err << std::endl;
    if (!res)
        std::cout << "Failed to load mesh: " << resourceName << std::endl;
}

MeshIf::~MeshIf() {
    glDeleteVertexArrays(1, &this->VAO);
}

void MeshIf::draw(Shader &shader) {
    glBindVertexArray(this->VAO);

    tinygltf::Mesh mesh = this->gltf.meshes.front();
    tinygltf::Primitive primitive = mesh.primitives.front();
    tinygltf::Accessor indexAccessor = this->gltf.accessors[primitive.indices];

    glDrawElements(
        primitive.mode,
        indexAccessor.count,
        indexAccessor.componentType,
        (char *) nullptr + (indexAccessor.byteOffset)
    );

    glBindVertexArray(0);
}

void MeshIf::bindMesh(tinygltf::Mesh &mesh, std::map<int, unsigned int> &VBOs) {
    const tinygltf::Buffer &buffer = this->gltf.buffers.front();
    const tinygltf::Primitive primitive = mesh.primitives.front();

    for (size_t i = 0; i < this->gltf.bufferViews.size(); ++i) {
        const tinygltf::BufferView &bufferView = this->gltf.bufferViews[i];
        if (bufferView.target == 0) {
            continue;
        }

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        VBOs[i] = VBO;
        glBindBuffer(bufferView.target, VBO);

        glBufferData(
                bufferView.target,
                bufferView.byteLength,
                &buffer.data.at(0) + bufferView.byteOffset,
                GL_STATIC_DRAW
        );
    }

    for (auto &attrib : primitive.attributes) {
        tinygltf::Accessor accessor = this->gltf.accessors[attrib.second];

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[accessor.bufferView]);

        int size = accessor.type;
        int byteStride = accessor.ByteStride(
                this->gltf.bufferViews[accessor.bufferView]
        );

        int vaa = -1;
        if (attrib.first.compare("POSITION") == 0)
            vaa = 0;
        if (attrib.first.compare("NORMAL") == 0)
            vaa = 1;
        if (attrib.first.compare("TEXCOORD_0") == 0)
            vaa = 2;
        if (attrib.first.compare("JOINTS_0") == 0)
            vaa = 3;
        if (attrib.first.compare("WEIGHTS_0") == 0)
            vaa = 4;

        glEnableVertexAttribArray(vaa);

        if (vaa != 3) {
            glVertexAttribPointer(
                    vaa, size,
                    accessor.componentType,
                    accessor.normalized ? GL_TRUE : GL_FALSE,
                    byteStride,
                    (char *) NULL + (accessor.byteOffset)
            );
        } else {
            glVertexAttribIPointer(
                    vaa, size,
                    accessor.componentType,
                    byteStride, (char *) NULL + (accessor.byteOffset)
            );
        }
    }
}


Mesh::Mesh(const std::string &resourceName) : MeshIf(resourceName, MeshType::STATIC) {
    this->bindModel();
}

void Mesh::bindModel() {
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

void Mesh::bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, glm::mat4 &globalTransform) {
    tinygltf::Node node = this->gltf.nodes[nodeId];

    if (node.mesh >= 0)
        this->bindMesh(this->gltf.meshes[node.mesh], VBOs);

    for (int i : node.children)
        bindModelNodes(nodeId, i, VBOs, globalTransform);
}
