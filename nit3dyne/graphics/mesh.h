#ifndef GL_MESH_H
#define GL_MESH_H

#include <glad/glad.h>
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

namespace n3d {

enum MeshType {
    STATIC,
    ANIMATED,
    COLORED
};

class Mesh {
public:
    explicit Mesh(const std::string &resourceName, MeshType meshType);
    virtual ~Mesh();

    virtual void draw(Shader &shader);

    MeshType meshType;

protected:
    void bindMesh(tinygltf::Mesh &mesh, std::map<int, unsigned int> &VBOs);

    unsigned int VAO;
    tinygltf::Model gltf;

private:
    int getVaa(const std::string &attrib);
    bool vaaIsInt(const std::string &attrib);
};

}

#endif // GL_MESH_H
