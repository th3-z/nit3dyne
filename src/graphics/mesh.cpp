#include "mesh.h"

Mesh::Mesh(const std::string &filename) {
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool res = loader.LoadBinaryFromFile(&this->gltf, &err, &warn, filename);
    if (!warn.empty())
        std::cout << "WARN: " << warn << std::endl;
    if (!err.empty())
        std::cout << "ERR: " << err << std::endl;
    if (!res)
        std::cout << "Failed to load GLTF file: " << filename << std::endl;

    this->bindBuffers();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Mesh::draw() {
    glBindVertexArray(this->VAO);

    tinygltf::Mesh mesh = this->gltf.meshes.front();
    tinygltf::Primitive primitive = mesh.primitives.front();
    tinygltf::Accessor indexAccessor = this->gltf.accessors[primitive.indices];

    glDrawElements(
        primitive.mode, indexAccessor.count, indexAccessor.componentType, (char *) NULL + (indexAccessor.byteOffset));

    glBindVertexArray(0);
}

void Mesh::bindBuffers() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    const tinygltf::Mesh mesh = this->gltf.meshes.front();
    const tinygltf::Buffer &buffer = this->gltf.buffers.front();
    const tinygltf::Primitive primitive = mesh.primitives.front();

    // Not using GLTF scenes, all files should have a single mesh
    assert(this->gltf.meshes.size() == 1);
    assert(this->gltf.buffers.size() == 1);
    assert(mesh.primitives.size() == 1);

    std::map<int, unsigned int> VBOs;

    for (size_t i = 0; i < this->gltf.bufferViews.size(); ++i) {
        const tinygltf::BufferView &bufferView = this->gltf.bufferViews[i];

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        VBOs[i] = VBO;
        glBindBuffer(bufferView.target, VBO);

        glBufferData(
            bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (auto &attrib : primitive.attributes) {
        tinygltf::Accessor accessor = this->gltf.accessors[attrib.second];
        int byteStride = accessor.ByteStride(this->gltf.bufferViews[accessor.bufferView]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[accessor.bufferView]);

        int size = accessor.type;

        int vaa = -1;
        if (attrib.first.compare("POSITION") == 0)
            vaa = 0;
        if (attrib.first.compare("NORMAL") == 0)
            vaa = 1;
        if (attrib.first.compare("TEXCOORD_0") == 0)
            vaa = 2;

        glEnableVertexAttribArray(vaa);
        glVertexAttribPointer(vaa,
                              size,
                              accessor.componentType,
                              accessor.normalized ? GL_TRUE : GL_FALSE,
                              byteStride,
                              (char *) NULL + (accessor.byteOffset));
    }

    glBindVertexArray(0);
    for (size_t i = 0; i < VBOs.size(); ++i) {
        glDeleteBuffers(1, &VBOs[i]);
    }
}
