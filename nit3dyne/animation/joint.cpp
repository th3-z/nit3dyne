#include "joint.h"

namespace n3d {

Joint::Joint(int id, glm::mat4 &inverseBindTransform) :
        id(id),
        inverseBindTransform(inverseBindTransform) {
}

void Joint::addChild(Joint *joint) {
    this->children.push_back(joint);
}

glm::mat4 Joint::getJointMatrix(glm::mat4 &globalTransform) const {
    return glm::inverse(globalTransform) * this->globalJointTransform * this->inverseBindTransform;
}

}
