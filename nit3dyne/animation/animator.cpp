#include "animator.h"

namespace n3d {

Animator::Animator(Skin *skin) : skin(skin) {}

void Animator::setAnimation(Animation &animation) {
    this->animationTime = 0.f;
    this->animation = &animation;
}

void Animator::update() {
    if (this->animation == nullptr) {
        return;
    }

    this->animationTime = std::fmod(
            animationTime + Display::timeDelta * this->speed, this->animation->length
    );

    for (Channel &c : this->animation->channels) {
        if (c.path == Path::ROTATION) {
            auto sampler = dynamic_cast<Sampler<vec4> *>(this->animation->samplers[c.sampler].get());
            vec4 xyzw = sampler->sample(this->animationTime);
            this->skin->jointById(c.joint)->rotation = normalize(quat(xyzw[3], xyzw[0], xyzw[1], xyzw[2]));
        } else if (c.path == Path::SCALE) {
            auto sampler = dynamic_cast<Sampler<vec3> *>(this->animation->samplers[c.sampler].get());
            this->skin->jointById(c.joint)->scale = sampler->sample(this->animationTime);
        } else if (c.path == Path::TRANSLATION) {
            auto sampler = dynamic_cast<Sampler<vec3> *>(this->animation->samplers[c.sampler].get());
            this->skin->jointById(c.joint)->translation = sampler->sample(this->animationTime);
        }
    }

    this->skin->updateGlobalJointMatrices();
}

}
