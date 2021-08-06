#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include "nit3dyne/core/math.h"

namespace n3d {

class Joint {
public:
    Joint(int id, mat4 &inverseBindTransform);

    void addChild(Joint *joint);

    mat4 getJointMatrix(mat4 &globalTransform) const;

    int id;

    mat4 inverseBindTransform;
    mat4 bindTransform;  // Bind pose
    mat4 globalJointTransform;  // Active pose

    // TODO: Add dirty check
    quat rotation;
    vec3 scale;
    vec3 translation;

    std::vector<Joint *> children;
};

}

#endif // JOINT_H
