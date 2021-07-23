#include "terrain.h"

Terrain::Terrain(std::string resourceName) {
    auto heights = this->readHeights("res/heightmap/" + resourceName + ".png", "res/heightmap/" + resourceName + ".normal.png");

    this->model = glm::translate(this->model, glm::vec3(-(100*50.f), -365.f, -(100*50.f)));
}

void Terrain::updateChunks(int x, int y) {

}

void Terrain::draw(Shader &shader, const glm::mat4 &perspective, const glm::mat4 &view) {
    shader.use();
    shader.attachMaterial(Materials::basic);

    glm::mat4 mvp = perspective * view * this->model;
    glm::mat4 modelView = view * this->model;
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
    stbi_set_flip_vertically_on_load(true);
    unsigned char *heightsData = stbi_load(heightsFn.c_str(), &w, &h, &c, 0); // FIXME: leaks

    auto *out = new std::vector<TerrainVertex>; // FIXME: leaks

    float range = 728.4f;
    float offset = 48.8f;
    float d = 50.f;

    for (int i = 0; i < w*h; ++i) {
        float vZ = (i % w) * d;
        float vX = floor(i / (float) w) * d;
        float vY = ((float) heightsData[i*3] * (range / 255.f)) + offset;

        float l = 0.f;
        float r = 0.f;
        float dw = 0.f;
        float u = 0.f;

        //if ((i+1) % w != 1)
            l = ((float) heightsData[i*3-3] * (range / 255.f)) + offset;
        //if ((i+1) % w > 0)
            r = ((float) heightsData[i*3+3] * (range / 255.f)) + offset;
        //if ((i+1) < w*(h-1))
            dw = ((float) heightsData[i*3+w*3] * (range / 255.f)) + offset;
        //if (i+1 > w)
            u = ((float) heightsData[i*3-w*3] * (range / 255.f)) + offset;

        glm::vec3 normal = glm::normalize(glm::vec3(u - dw, 100.f, l - r));
        //std::cout <<i <<":"<< l <<","<<r<<","<<u<<","<<dw<<std::endl;

        out->emplace_back(
                TerrainVertex{
                    .vertex = {vX, vY, vZ},
                    .normal = normal, // std::move?
                    .texture = 1,
                    .uv = glm::vec2(
                            (float) (i % w) * (1.f / (w - 1)),
                            1.f - floor(i / (float) w) * (1.f / (w - 1))
                    )
                }
        );

        if ((i+1) % w == 0) continue;
        if ((i+1) > w*(h-1)) continue;

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

    glEnableVertexAttribArray(3);
    glVertexAttribPointer( // UV
            3, 2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(TerrainVertex),
            (void*) offsetof(TerrainVertex, uv)
    );

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);

    return out;

    glBindVertexArray(0);
}
