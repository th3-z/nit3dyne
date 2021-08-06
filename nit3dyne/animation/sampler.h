#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include "nit3dyne/core/math.h"

namespace n3d {

enum Interpolation {
    LINEAR = 1,
    STEP,
    CUBICSPLINE
};

class SamplerIf {
public:
    virtual ~SamplerIf() = default;

    std::vector<float> times;
    Interpolation interpolation;
};


template<typename T>
class Sampler : public SamplerIf {
public:
    Sampler(std::vector<float> &times, std::vector<T> &samples, Interpolation interpolation);

    std::vector<T> samples;

    T sample(float time);

    T cubicSpline(T prevPoint, T prevTang, T nextPoint, T nextTang, float t);
};

template<typename T>
Sampler<T>::Sampler(std::vector<float> &times, std::vector<T> &samples, Interpolation interpolation):
        samples(samples) {
    this->interpolation = interpolation;
    this->times = times;
}

template<typename T>
T Sampler<T>::cubicSpline(T prevPoint, T prevTang, T nextPoint, T nextTang, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    return (2 * t3 - 3 * t2 + 1) * prevPoint
           + (t3 - 2 * t2 + t) * prevTang
           + (-2 * t3 + 3 * t2) * nextPoint
           + (t3 - t2) * nextTang;
}

template<typename T>
T Sampler<T>::sample(float time) {
    int kPrevious = 0;
    for (; kPrevious < this->times.size() - 1; ++kPrevious) {
        if (this->times[kPrevious + 1] > time) break;
    }

    int kNext = (time < this->times[kPrevious]) ?
                kPrevious :
                std::min(kPrevious + 1, (int) this->times.size() - 1);

    int vPrev = 1 + kPrevious * 3;
    int vNext = 1 + kNext * 3;

    float deltaTime = this->times[kNext] - this->times[kPrevious];
    float interpolation = deltaTime == 0 ?
                          0 :
                          (time - this->times[kPrevious]) / deltaTime;

    return this->cubicSpline(
            this->samples[vPrev], this->samples[vPrev + 1] * deltaTime, this->samples[vNext],
            this->samples[vNext - 1] * deltaTime, interpolation
    );
}

}

#endif // SAMPLER_H
