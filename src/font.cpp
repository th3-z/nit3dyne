//
// Created by the_z on 14/05/2021.
//

#include "font.h"

const char* FONT_FILE = "res/textures/font.png";
const unsigned int FONT_WIDTH = 5;
const unsigned int FONT_HEIGHT = 8;
const unsigned int FONT_WIDTHS[95] = {
        // SPACE !"#%&'()*+,-./
        5, 1, 3, 5, 5, 5, 5, 1, 2, 2, 3, 3, 2, 3, 1, 4,
        // 0 - 9
        4, 3, 4, 4, 4, 4, 4, 4, 4, 4,
        // :;<=>?@
        1, 1, 4, 4, 4, 4, 5,
        // A - Z
        4, 4, 4, 4, 4, 4, 4, 4, 3, 5, 4, 4, 5, 5, 4, 4, 5, 4, 4, 5, 4, 5, 5, 5, 5, 5,
        // [\]^_`
        2, 4, 2, 3, 5, 2,
        // a - z
        4, 4, 4, 4, 4, 2, 4, 4, 1, 2, 3, 2, 5, 4, 4, 4, 4, 3, 4, 3, 4, 3, 5, 5, 4, 4,
        // {|}~
        2, 1, 2, 5
};

Font::Font(const char *str) {
    std::string tPath(FONT_FILE);
    this->texture = new Texture(tPath);

    float strWidth = 0.f;

    for (int i = 0; str[i] != '\0'; ++i) {
        int charIdx = str[i] - ' ';
        float charWidth = (1.f / FONT_WIDTH) * FONT_WIDTHS[charIdx];
        float charHeight = 1.f;

        std::pair<float, float> uvTL = this->getTexelCoord(
            charIdx*FONT_WIDTH, FONT_HEIGHT
        );
        std::pair<float, float> uvBR = this->getTexelCoord(
                charIdx*FONT_WIDTH + FONT_WIDTHS[charIdx], 0
        );

        vertices.push_back(strWidth + 0.f);  // TL
        vertices.push_back(charHeight);
        uvs.push_back(uvTL.first);
        uvs.push_back(uvTL.second);

        vertices.push_back(strWidth + 0.f); // BL
        vertices.push_back(0.f);
        uvs.push_back(uvTL.first);
        uvs.push_back(uvBR.second);

        vertices.push_back(strWidth + charWidth);  // TR
        vertices.push_back(charHeight);
        uvs.push_back(uvBR.first);
        uvs.push_back(uvTL.second);

        vertices.push_back(strWidth + charWidth);  // BR
        vertices.push_back(0.f);
        uvs.push_back(uvBR.first);
        uvs.push_back(uvBR.second);

        indices.push_back(0 + i*4);
        indices.push_back(1 + i*4);
        indices.push_back(2 + i*4);
        indices.push_back(2 + i*4);
        indices.push_back(1 + i*4);
        indices.push_back(3 + i*4);

        strWidth += charWidth + 0.3f;
    }

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0
    );

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(float), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0
    );

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    this->shader = new Shader("shaders/font.vert","shaders/font.frag");
    this->shader->use();
    this->shader->setUniform("tex", 0);
}

void Font::draw() {
    this->shader->use();
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, this->texture->handle);

    glBindVertexArray(this->VAO);
    glDrawElements(
        GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0
    );
    glBindVertexArray(0);
}

std::pair<float, float> Font::getTexelCoord(int x, int y) {
    return std::pair<float, float>(
        (x ) /(float) this->texture->w,
        (y ) /(float) this->texture->h
    );
}
