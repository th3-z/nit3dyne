#include "gltf_utils.h"

namespace n3d {

void emplaceData(float *src, std::vector<float> &dst) {
    dst.emplace_back(
            *src
    );
}

void emplaceData(float *src, std::vector<vec3> &dst) {
    dst.emplace_back(
            make_vec3(src)
    );
}

void emplaceData(float *src, std::vector<vec4> &dst) {
    dst.emplace_back(
            make_vec4(src)
    );
}

void emplaceData(float *src, std::vector<mat4> &dst) {
    dst.emplace_back(
            make_mat4(src)
    );
}

}
