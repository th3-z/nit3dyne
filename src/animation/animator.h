#ifndef GL_ANIMATOR_H
#define GL_ANIMATOR_H


#include "skin.h"
#include "animation.h"
#include "../display.h"

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


#endif //GL_ANIMATOR_H
