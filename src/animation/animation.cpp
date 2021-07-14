#include "animation.h"

Animation::Animation(tinygltf::Model &gltf, tinygltf::Animation &animation, Skin &skin) {
    for (auto &channel : animation.channels) {
        Path path;
        if (channel.target_path == "translation")
            path = Path::TRANSLATION;
        else if (channel.target_path == "rotation")
            path = Path::ROTATION;
        else if (channel.target_path == "scale")
            path = Path::SCALE;
        else
            path = Path::WEIGHTS;

        this->channels.emplace_back(Channel{
            skin.jointByNode(channel.target_node)->id, path, channel.sampler
        });
    }

    for (int i = 0; i < animation.samplers.size(); ++i) {
        tinygltf::AnimationSampler &sampler = animation.samplers[i];

        Interpolation interpolation;
        if (sampler.interpolation == "CUBICSPLINE")
            interpolation = Interpolation::CUBICSPLINE;
        else if (sampler.interpolation == "STEP")
            interpolation = Interpolation::STEP;
        else
            interpolation = Interpolation::LINEAR;

        tinygltf::Accessor &inputAccessor = gltf.accessors[sampler.input];
        tinygltf::Accessor &outputAccessor = gltf.accessors[sampler.output];

        std::vector<float> times;
        readBuffer<float>(inputAccessor, gltf, times);

        if (samplerPath(this->channels, i) == Path::ROTATION){
            std::vector<glm::vec4> data;
            readBuffer<glm::vec4>(outputAccessor, gltf, data);
            this->samplers.push_back(std::make_unique<Sampler<glm::vec4>>(
                times, data, interpolation)
            );
        } else {
            std::vector<glm::vec3> data;
            readBuffer<glm::vec3>(outputAccessor, gltf, data);
            this->samplers.push_back(std::make_unique<Sampler<glm::vec3>>(
                times, data, interpolation)
            );
        }
    }

    for (auto &sampler : this->samplers) {
        if (sampler->times.back() > this->length)
            this->length = sampler->times.back();
    }
}

inline Path samplerPath(std::vector<Channel> &channels, int sampler) {
    for (auto &channel : channels) {
        if (channel.sampler == sampler) {
            return channel.path;
        }
    }
    assert(0);
}


