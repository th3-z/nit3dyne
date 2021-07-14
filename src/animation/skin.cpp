#include "skin.h"

Joint *Skin::jointByNode(int nodeId) {
    for (auto &jPair : this->joints) {
        if (jPair.first == nodeId)
            return &jPair.second;
    }
    return nullptr;
}

Joint *Skin::jointById(int jointId) {
    assert(this->joints.size() > jointId);
    return &this->joints[jointId].second;
}

bool Skin::isJoint(int nodeId) {
    return std::any_of(
        this->joints.begin(), this->joints.end(),
        [nodeId] (auto &j) {return j.first == nodeId;}
    );
}

void Skin::updateGlobalJointMatrices() {
    this->updateGlobalJointMatrices(this->rootJoint, this->globalTransform);
}
void Skin::updateGlobalJointMatrices(Joint *joint, glm::mat4 &parentTransform) {
    joint->globalJointTransform = glm::scale(
        glm::translate(parentTransform, joint->translation) * glm::toMat4(joint->rotation),
        joint->scale
    );

    for (Joint *child : joint->children) {
        this->updateGlobalJointMatrices(child, joint->globalJointTransform);
    }
}
