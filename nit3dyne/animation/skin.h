#ifndef GL_SKIN_H
#define GL_SKIN_H

#include "nit3dyne/animation/joint.h"
#include "nit3dyne/core/math.h"
#include <algorithm>
#include <vector>

namespace n3d {

class Skin {
public:
    mat4 globalTransform;
    std::vector<std::pair<int, Joint>> joints;
    Joint *rootJoint = nullptr;

    void updateGlobalJointMatrices();

    Joint *jointByNode(int nodeId);

    Joint *jointById(int jointId);

    bool isJoint(int nodeId);

private:
    void updateGlobalJointMatrices(Joint *joint, mat4 &parentTransform);
};

}

#endif // GL_SKIN_H
