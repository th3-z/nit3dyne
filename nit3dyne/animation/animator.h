#ifndef GL_ANIMATOR_H
#define GL_ANIMATOR_H


#include "nit3dyne/animation/skin.h"
#include "nit3dyne/animation/animation.h"
#include "nit3dyne/core/display.h"

namespace n3d {

class Animator {
public:
    Animator() = default;

    explicit Animator(Skin *skin);

    void update();

    void setAnimation(Animation &animation);

private:
    Skin *skin = nullptr;
    Animation *animation = nullptr;

    float animationTime = 0.f;
    float speed = 1.f;
    bool isPaused = true;
};

}

#endif //GL_ANIMATOR_H
