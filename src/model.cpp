//
// Created by the_z on 05/05/2021.
//

#include "model.h"

Model::Model(std::vector<Vertex> &vertices, std::vector<int> &indices, std::vector<Texture> &textures):
vertices(vertices),
indices(indices),
textures(textures) {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,  // Location
        3,  // size (x/y/z)
        GL_FLOAT,  // Type
        GL_FALSE,  // Normalized
        sizeof(Vertex),  // Stride, walk over `Vertex`
        (void*)offsetof(Vertex, position)  // Start, offsetof to find member in `Vertex`
    );
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model::~Model() {
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);
}

void Model::render(Shader &shader) {
    glUseProgram(shader.handle);
    shader.setVec3("aColor", this->color);

    int nDiffuse = 0;
    int nSpecular = 0;
    int nNormal = 0;

    for (int i = 0;  i < this->textures.size(); i++) {
        std::string number;
        if (this->textures[i].type == "diffuse")
            number = std::to_string(nDiffuse++);
        else if (this->textures[i].type == "specular")
            number = std::to_string(nSpecular++);
        else if (this->textures[i].type == "normal")
            number = std::to_string(nNormal++);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].handle);
        shader.setInt(this->textures[i].type + number, i);
    }

    glBindVertexArray(this->VAO);
    glDrawElements(
        GL_TRIANGLES,  // Draw mode
        this->indices.size(),  // Number of indices
        GL_UNSIGNED_INT,  // Indice type
        0  // Offset in bound EBO
    );

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}
