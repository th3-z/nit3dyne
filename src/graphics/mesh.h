#ifndef GL_MESH_H
#define GL_MESH_H

#include <glad/glad.h>
#ifndef NDEBUG
#include <iostream>
#endif
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../animation/animation.h"
#include "../animation/joint.h"
#include "../utils/gltf_utils.h"
#include "../animation/skin.h"
#include "shader.h"
#include "tiny_gltf.h"
#include <cmath>

enum MeshType {
    STATIC,
    ANIMATED
};

class MeshIf {
public:
    explicit MeshIf(const std::string &resourceName, MeshType meshType);
    virtual ~MeshIf();

    virtual void draw(Shader &shader);

    MeshType meshType;
protected:
    void bindMesh(tinygltf::Mesh &mesh, std::map<int, unsigned int> &VBOs);

    unsigned int VAO;
    tinygltf::Model gltf;
};


class Mesh: public MeshIf {
public:
    explicit Mesh(const std::string &resourceName);
    ~Mesh() override = default;

private:
    void bindModel();
    void bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, glm::mat4 &globalTransform);
};

#endif // GL_MESH_H
