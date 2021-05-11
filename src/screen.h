//
// Created by the_z on 11/05/2021.
//

#ifndef GL_SCREEN_H
#define GL_SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Screen {
public:
    Screen(int w, int h, float fov, const char* title);
    ~Screen();

    void resize();
    void flip();

    int w;
    int h;
    float fov;
    glm::mat4 perspective;
private:
    SDL_Window *window;
    SDL_GLContext context;
};


#endif //GL_SCREEN_H
