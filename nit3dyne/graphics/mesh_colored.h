#ifndef NIT3DYNE_EX_MESH_COLORED_H
#define NIT3DYNE_EX_MESH_COLORED_H

#include "nit3dyne/core/math.h"
#include <string>

#include "nit3dyne/graphics/mesh.h"

namespace n3d {

class MeshColored : public Mesh {
public:
    explicit MeshColored(const std::string &resourceName);
    ~MeshColored() override = default;

private:
    void bindModel();
    void bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, mat4 &globalTransform);
};


}

#endif //NIT3DYNE_EX_MESH_COLORED_H
