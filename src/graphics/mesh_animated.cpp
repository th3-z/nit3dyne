#include "mesh_animated.h"

MeshAnimated::MeshAnimated(const std::string &resourceName) : MeshIf(resourceName, MeshType::ANIMATED) {
    this->bindModel();

    for (auto &animation : this->gltf.animations) {
        this->animations.emplace_back(Animation(this->gltf, animation, this->skin));
    }

    this->animator = Animator(&this->skin);
    this->animator.setAnimation(this->animations.front());
}

void MeshAnimated::bindModel() {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    std::map<int, unsigned int> VBOs;
    glm::mat4 globalTransform(1.f);

    this->bindModelNodes(
            -1,
            this->gltf.scenes[this->gltf.defaultScene].nodes.front(),
            VBOs, globalTransform
    );

    // Cleanup
    glBindVertexArray(0);
    for (size_t i = 0; i < VBOs.size(); ++i) {
        glDeleteBuffers(1, &VBOs[i]);
    }
}

void MeshAnimated::bindModelNodes(int parentId, int nodeId, std::map<int, unsigned int> &VBOs, glm::mat4 &globalTransform) {
    tinygltf::Node node = this->gltf.nodes[nodeId];

    glm::quat rotation(1.f, 0.f, 0.f, 0.f);
    if (!node.rotation.empty())
        rotation = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);

    glm::vec3 scale(1.f, 1.f, 1.f);
    if (!node.scale.empty())
        scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);

    glm::vec3 translation(0.f, 0.f, 0.f);
    if (!node.translation.empty())
        translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);

    glm::mat4 nodeTransform = glm::scale(
            glm::translate(globalTransform, translation) * glm::toMat4(rotation),
            scale
    );

    if (node.skin >= 0) this->bindSkin(this->gltf.skins[node.skin], nodeTransform);
    if (node.mesh >= 0) this->bindMesh(this->gltf.meshes[node.mesh], VBOs);

    // Bind joints, assumes skin already found
    if (this->skin.isJoint(nodeId)) {
        Joint *joint = this->skin.jointByNode(nodeId);
        joint->translation = translation;
        joint->scale = scale;
        joint->rotation = rotation;
        joint->bindTransform = nodeTransform;

        if (this->skin.isJoint(parentId)) {
            this->skin.jointByNode(parentId)->addChild(joint);
        } else {
            this->skin.rootJoint = this->skin.jointByNode(nodeId);
        }
    }

    // Recur down node tree
    for (int i : node.children)
        bindModelNodes(nodeId, i, VBOs, nodeTransform);
}

void MeshAnimated::bindSkin(tinygltf::Skin &skin, glm::mat4 &globalTransform) {
    tinygltf::Accessor &ibmAccessor = this->gltf.accessors[skin.inverseBindMatrices];
    tinygltf::BufferView &ibmView = this->gltf.bufferViews[ibmAccessor.bufferView];
    std::vector<glm::mat4> inverseBindMats;
    readBuffer<glm::mat4>(ibmAccessor, this->gltf, inverseBindMats);

    for (size_t i = 0; i < skin.joints.size(); ++i) {
        this->skin.joints.emplace_back(std::pair<int, Joint>(skin.joints[i], Joint(i, inverseBindMats[i])));
    }

    this->skin.globalTransform = globalTransform;
}

void MeshAnimated::draw(Shader &shader) {
    this->animator.update();

    std::vector<glm::mat4> jointMatrices;
    for (auto &pJoint : this->skin.joints) {
        jointMatrices.push_back(pJoint.second.getJointMatrix(this->skin.globalTransform));
    }

    shader.setUniform("jointTransforms", jointMatrices);

    MeshIf::draw(shader);
}
