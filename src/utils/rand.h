#ifndef GL_RAND_H
#define GL_RAND_H

#include <cstdlib>

inline float randFloat(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (min - max)));
}

#endif //GL_RAND_H
