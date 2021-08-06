#include "model.h"

namespace n3d {

Model::Model(const std::shared_ptr<Mesh> mesh, const std::shared_ptr<Texture> texture) :
        modelMat(mat4(1.f)), mesh(mesh), texture(texture) {
}

Model::~Model() = default;

void Model::draw(Shader &shader, const mat4 &perspective, const mat4 &view) {
    shader.use();
    shader.attachMaterial(*this->material);

    mat4 mvp = perspective * view * this->modelMat;
    mat4 modelView = view * this->modelMat;
    mat3 normalMat = inverse(transpose(mat3(modelView)));

    shader.setUniform("mvp", mvp);
    shader.setUniform("modelView", modelView);
    shader.setUniform("normalMat", normalMat);

    if (this->mesh->meshType != MeshType::COLORED) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture->handle);
    }

    if (this->mesh->meshType == MeshType::ANIMATED) {
        dynamic_cast<MeshAnimated *>(this->mesh.get())->draw(shader);
    } else if (this->mesh->meshType == MeshType::STATIC) {
        dynamic_cast<MeshStatic *>(this->mesh.get())->draw(shader);
    } else if (this->mesh->meshType == MeshType::COLORED) {
        dynamic_cast<MeshColored *>(this->mesh.get())->draw(shader);
    }
}

void Model::translate(float x, float y, float z) {
    this->modelMat = n3d::translate(this->modelMat, vec3(x, y, z));
}

void Model::scale(float x, float y, float z) {
    this->modelMat = n3d::scale(this->modelMat, vec3(x, y, z));
}

void Model::rotate(float deg, float x, float y, float z, bool normalize) {
    this->modelMat = n3d::rotate(this->modelMat,
                                 radians(deg),
                                 normalize ? n3d::normalize(vec3(x, y, z)) : vec3(x, y, z));
}

void Model::setMaterial(const Material &material) {
    this->material = &material;
}

}