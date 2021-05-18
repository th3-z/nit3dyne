//
// Created by the_z on 18/05/2021.
//

#include "model.h"

Model::Model(const std::string &meshFilename, const std::string &textureFilename):
mesh(meshFilename), texture(textureFilename), modelMat(glm::mat4(1.f)) {
}

Model::~Model() = default;

void Model::draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view) {
    shader.use();

    glm::mat4 mvp = perspective * view * this->modelMat;
    glm::mat4 modelView = view * this->modelMat;
    glm::mat3 normalMat = glm::inverse(
            glm::transpose(glm::mat3(modelView))
    );

    shader.setUniform("mvp", mvp);
    shader.setUniform("modelView", modelView);
    shader.setUniform("normalMat", normalMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture.handle);
    this->mesh.draw();
}

void Model::translate(float x, float y, float z) {
    this->modelMat = glm::translate(this->modelMat, glm::vec3(x, y, z));
}

void Model::scale(float x, float y, float z) {
    this->modelMat = glm::scale(this->modelMat, glm::vec3(x, y, z));
}

void Model::rotate(float deg, float x, float y, float z, bool normalize) {
    this->modelMat = glm::rotate(
        this->modelMat, glm::radians(deg), normalize?
            glm::normalize(glm::vec3(x, y, z)):
            glm::vec3(x, y, z)
    );
}
