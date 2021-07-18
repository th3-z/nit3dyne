#include "terrain.h"

Terrain::Terrain(std::string resourceName) {
    auto heights = this->readHeights("res/heightmap/" + resourceName + ".png", "res/heightmap/" + resourceName + ".normal.png");
}

void Terrain::updateChunks(int x, int y) {

}

void Terrain::draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view) {
    shader.use();
    shader.attachMaterial(Materials::basic);

    glm::mat4 mvp = perspective * view * glm::mat4(1.f);
    glm::mat4 modelView = view * glm::mat4(1.f);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(modelView)));

    shader.setUniform("mvp", mvp);
    shader.setUniform("modelView", modelView);
    shader.setUniform("normalMat", normalMat);

    glBindVertexArray(this->VAO);
    glDrawElements(
            GL_TRIANGLES,
            this->indices.size(),
            GL_UNSIGNED_INT,
            0
    );

    glBindVertexArray(0);
}

std::vector<TerrainVertex> *Terrain::readHeights(std::string heightsFn, std::string normalsFn) {
    int c, w, h;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *heightsData = stbi_load(heightsFn.c_str(), &w, &h, &c, 0); // FIXME: leaks
    unsigned char *normalsData = stbi_load(heightsFn.c_str(), &w, &h, &c, 0); // FIXME: leaks

    auto *out = new std::vector<TerrainVertex>; // FIXME: leaks

    float range = 728.4f;
    float offset = 48.8f;
    float d = 50.f;

    for (int i = 0; i < w*h; ++i) {
        float vX = (w*d) - (i % w) * d;
        float vZ = (w*d) - ceil((float) i / (float) w) * d;
        float vY = ((float) heightsData[i*3] * (range / 255.f)) + offset;

        std::cout <<i <<":"<< vX <<","<<vY<<","<<vZ<<std::endl;

        float nR = ((float) normalsData[i*3] / 255) * 2.f - 1.f;
        float nG = ((float) normalsData[i*3+1] / 255) * 2.f - 1.f;
        float nB = ((float) normalsData[i*3+2] / 255) * 2.f - 1.f;
        glm::vec3 normal = glm::normalize(glm::vec3(nR, nG, nB));

        out->emplace_back(
                TerrainVertex{
                    .vertex = {vX, vY, vZ},
                    .normal = normal, // std::move?
                    .texture = 1,
                }
        );

        if ((i+1) % w == 0) continue;
        if ((i+1) > w*(h-1)) continue;

        //std::cout<<i<<": " <<i % w<<","<<ceil(i/w)<<std::endl;

        this->indices.push_back(i);
        this->indices.push_back(i+w);
        this->indices.push_back(i+1);
        this->indices.push_back(i+1);
        this->indices.push_back(i+w);
        this->indices.push_back(i+w+1);
    }

    std::cout<<this->indices.size()<<std::endl;
    std::cout<<out->size()<<std::endl;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(TerrainVertex) * out->size(),
            &out->front(),
            GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0); // POS
    glVertexAttribPointer(
            0, 3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(TerrainVertex),
            (void*) 0
    );

    glEnableVertexAttribArray(1); // NORMAL
    glVertexAttribPointer(
            1, 3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(TerrainVertex),
            (void*) offsetof(TerrainVertex, normal)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer( // TEXT
            2, 1,
            GL_INT,
            sizeof(TerrainVertex),
            (void*) offsetof(TerrainVertex, texture)
    );

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);

    return out;

    glBindVertexArray(0);
}
