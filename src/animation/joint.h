#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

class Joint {
public:
    Joint(int id, glm::mat4 &inverseBindTransform);
    void addChild(Joint *joint);
    glm::mat4 getJointMatrix(glm::mat4 &globalTransform) const;

    int id;

    glm::mat4 inverseBindTransform;
    glm::mat4 bindTransform;  // Bind pose
    glm::mat4 globalJointTransform;  // Active pose

    // Add dirty check
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 translation;

    std::vector<Joint *> children;
};

#endif // JOINT_H
