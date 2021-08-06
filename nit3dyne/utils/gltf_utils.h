#ifndef GL_GLTF_UTILS_H
#define GL_GLTF_UTILS_H

#include <tiny_gltf.h>
#include <vector>

#include "nit3dyne/core/math.h"

namespace n3d {

void emplaceData(float *src, std::vector<float> &dst);

void emplaceData(float *src, std::vector<vec3> &dst);

void emplaceData(float *src, std::vector<vec4> &dst);

void emplaceData(float *src, std::vector<mat4> &dst);

template<typename T>
void readBuffer(tinygltf::Accessor &accessor, tinygltf::Model &model, std::vector<T> &data) {
    tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
    tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

    auto it = buffer.data.cbegin() + bufferView.byteOffset;
    int end = bufferView.byteOffset + bufferView.byteLength;
    int stride = tinygltf::GetComponentSizeInBytes(accessor.componentType)
                 * tinygltf::GetNumComponentsInType(accessor.type);

    for (; it < buffer.data.cbegin() + end; it += stride) {
        emplaceData((float *) &(*it), data);
    }
}

}

#endif // GL_GLTF_UTILS_H
