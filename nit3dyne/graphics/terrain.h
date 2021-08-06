#ifndef GL_TERRAIN_H
#define GL_TERRAIN_H


#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <stb_image.h>

#include "nit3dyne/core/math.h"
#include "nit3dyne/graphics/shader.h"

namespace n3d {

// 28 bytes
struct TerrainVertex {
    vec3 vertex;
    vec3 normal; // TODO: precompute normals as normalmap
    unsigned int texture;
    vec2 uv;
};

/*
 * 128 x 128 verts per chunk (~400K)
 * verts seperated by 0.5 units of space
 */

//class TerrainChunk {
//    TerrainChunk(std::vector<float> heights);
//    ~TerrainChunk();
//
//    void bind();
//    void draw();
//
//    std::vector<TerrainVertex> geometry = std::vector<TerrainVertex>(16384);
//    // FIXME: these two should be outside this class to avoid duplication
//    int width = 128;
//    float maxHeight = 64.f;
//    std::pair<int, int> location;
//    unsigned int VAO;
//
//};

class Terrain {
public:
    Terrain(std::string resourceName);

    //std::vector<TerrainChunk> chunks;

    // unload out of range chunks, load in-range chunks
    void updateChunks(int x, int y);

    int width = 7;  // 49 chunks centered on location, total size ~2.2M, 800K verts, 192 meter draw dist
    unsigned int VAO; // TODO: remove
    std::vector<int> indices;
    // FIXME: draw dist exceeds the far clip pane, halve the size of chunks

    // draw all chunks
    void draw(Shader &shader, const mat4 &perspective, const mat4 &view);

    mat4 model = mat4(1.f);

    std::vector<TerrainVertex> *readHeights(std::string heightsFn, std::string normalsFn);
};

}

#endif //GL_TERRAIN_H
