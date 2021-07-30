#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <memory>

#include "../utils/gltf_utils.h"
#include "sampler.h"
#include "skin.h"
#include "tiny_gltf.h"

namespace n3d {

enum Path {
    TRANSLATION = 1,
    ROTATION,
    SCALE,
    WEIGHTS  // TODO: Morph targets
};

struct Channel {
    int joint;
    Path path;
    int sampler;
};

inline Path samplerPath(std::vector<Channel> &channels, int sampler);

class Animation {
public:
    Animation(tinygltf::Model &gltf, tinygltf::Animation &animation, Skin &skin);

    std::vector<Channel> channels;
    std::vector<std::unique_ptr<SamplerIf>> samplers;

    float length = 0.f;
};

}

#endif // ANIMATION_H
