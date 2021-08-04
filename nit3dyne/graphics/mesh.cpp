#include "mesh.h"

namespace n3d {

std::string ext = ".glb";
std::string path = "res/mesh/";

Mesh::Mesh(const std::string &resourceName, MeshType meshType) :
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

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->VAO);
}

void Mesh::draw(Shader &shader) {
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

void Mesh::bindMesh(tinygltf::Mesh &mesh, std::map<int, unsigned int> &VBOs) {
    const tinygltf::Buffer &buffer = this->gltf.buffers.front();
    const tinygltf::Primitive primitive = mesh.primitives.front();

    for (size_t i = 0; i < this->gltf.bufferViews.size(); ++i) {
        const tinygltf::BufferView &bufferView = this->gltf.bufferViews[i];
        if (bufferView.target == 0) continue;

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

        int vaa = this->getVaa(attrib.first);
        if (vaa == -1) continue;

        glEnableVertexAttribArray(vaa);
        if (this->vaaIsInt(attrib.first))
            glVertexAttribIPointer(
                vaa, size,
                accessor.componentType,
                byteStride, (char *) NULL + (accessor.byteOffset)
            );
        else
            glVertexAttribPointer(
                vaa, size,
                accessor.componentType,
                accessor.normalized ? GL_TRUE : GL_FALSE,
                byteStride,
                (char *) NULL + (accessor.byteOffset)
            );
    }
}

int Mesh::getVaa(const std::string &attrib) {
    int vaa = -1;

    switch (this->meshType) {
        case COLORED:
            if (attrib.compare("POSITION") == 0)
                vaa = 0;
            if (attrib.compare("NORMAL") == 0)
                vaa = 1;
            if (attrib.compare("COLOR_0") == 0)
                vaa = 2;
            break;
        case ANIMATED:
            if (attrib.compare("POSITION") == 0)
                vaa = 0;
            if (attrib.compare("NORMAL") == 0)
                vaa = 1;
            if (attrib.compare("TEXCOORD_0") == 0)
                vaa = 2;
            if (attrib.compare("JOINTS_0") == 0)
                vaa = 3;
            if (attrib.compare("WEIGHTS_0") == 0)
                vaa = 4;
            break;
        case STATIC:
            if (attrib.compare("POSITION") == 0)
                vaa = 0;
            if (attrib.compare("NORMAL") == 0)
                vaa = 1;
            if (attrib.compare("TEXCOORD_0") == 0)
                vaa = 2;
            break;
    }

    return vaa;
}

bool Mesh::vaaIsInt(const std::string &attrib) {
    if (attrib.compare("JOINTS_0") == 0)
        return true;
    return false;
}

}
