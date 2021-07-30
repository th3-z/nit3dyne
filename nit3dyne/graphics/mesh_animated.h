#ifndef GL_MESH_ANIMATED_H
#define GL_MESH_ANIMATED_H

#include "mesh.h"
#include "../animation/animator.h"
#include <iostream>

namespace n3d {

class MeshAnimated : public MeshIf {
public:
    explicit MeshAnimated(const std::string &resourceName);

    ~MeshAnimated() override = default;

    void draw(Shader &shader) override;

    void changeAnim();

private:
    void bindModel();

    void bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, glm::mat4 &globalTransform);

    void bindSkin(tinygltf::Skin &skin, glm::mat4 &globalTransform);

    Skin skin;
    std::vector<Animation> animations;
    Animator animator;
};

#endif // GL_MESH_ANIMATED_H

}