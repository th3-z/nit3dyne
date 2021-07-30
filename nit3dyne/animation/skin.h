#ifndef GL_SKIN_H
#define GL_SKIN_H

#include "joint.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

namespace n3d {

class Skin {
public:
    glm::mat4 globalTransform;
    std::vector<std::pair<int, Joint>> joints;
    Joint *rootJoint = nullptr;

    void updateGlobalJointMatrices();

    Joint *jointByNode(int nodeId);

    Joint *jointById(int jointId);

    bool isJoint(int nodeId);

private:
    void updateGlobalJointMatrices(Joint *joint, glm::mat4 &parentTransform);
};

}

#endif // GL_SKIN_H
