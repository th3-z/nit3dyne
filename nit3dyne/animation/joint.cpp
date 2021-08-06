#include "joint.h"

namespace n3d {

Joint::Joint(int id, mat4 &inverseBindTransform) :
        id(id),
        inverseBindTransform(inverseBindTransform) {
}

void Joint::addChild(Joint *joint) {
    this->children.push_back(joint);
}

mat4 Joint::getJointMatrix(mat4 &globalTransform) const {
    return inverse(globalTransform) * this->globalJointTransform * this->inverseBindTransform;
}

}
