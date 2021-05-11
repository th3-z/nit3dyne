//
// Created by the_z on 11/05/2021.
//

#include "input.h"

Input::Input(Camera **camera):
camera(camera), event({0}), quit(false) {

}

Input::~Input() {
    delete *(this->camera);
}

void Input::handleEvents(float timeDelta) {
    int directions = 0;
    int mX = 0;
    int mY = 0;

    //continuous-response directions
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_W]) directions |= Direction::FORWARD;
    if(keystate[SDL_SCANCODE_A]) directions |= Direction::LEFT;
    if(keystate[SDL_SCANCODE_S]) directions |= Direction::BACKWARD;
    if(keystate[SDL_SCANCODE_D]) directions |= Direction::RIGHT;
    if(keystate[SDL_SCANCODE_SPACE]) directions |= Direction::UP;
    if(keystate[SDL_SCANCODE_LSHIFT]) directions |= Direction::DOWN;

    while (SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
            case SDL_QUIT:
                this->quit = true;
                break;
            case SDL_MOUSEMOTION:
                mX = this->event.motion.xrel;
                mY = this->event.motion.yrel;
                break;
            case SDL_KEYDOWN:
                switch (this->event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        this->quit = true;
                        break;
                    case SDLK_1:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        break;
                    case SDLK_2:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        break;
                    case SDLK_f:
                        delete *(this->camera);
                        *(this->camera) = new CameraFixed;
                        break;
                    case SDLK_g:
                        delete *(this->camera);
                        *(this->camera) = new CameraFree;
                        break;
                }
                break;
        }
    }

    (*this->camera)->handleDirection(directions, timeDelta);
    (*this->camera)->handleMouse(mX, mY);
}
