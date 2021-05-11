//
// Created by the_z on 11/05/2021.
//

#ifndef GL_INPUT_H
#define GL_INPUT_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "camera/camera.h"
#include "camera/cameraFixed.h"
#include "camera/cameraFree.h"

class Input {
public:
    Input(Camera **camera);
    ~Input();

    void handleEvents(float timeDelta);

    bool quit;

private:
    Camera **camera;
    SDL_Event event;
};


#endif //GL_INPUT_H
