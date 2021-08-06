#ifndef NIT3DYNE_EX_MESH_STATIC_H
#define NIT3DYNE_EX_MESH_STATIC_H

#include "nit3dyne/graphics/mesh.h"

namespace n3d{

class MeshStatic : public Mesh {
public:
    explicit MeshStatic(const std::string &resourceName);
    ~MeshStatic() override = default;

private:
    void bindModel();

};

}

#endif //NIT3DYNE_EX_MESH_STATIC_H
