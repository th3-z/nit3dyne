//
// Created by the_z on 11/05/2021.
//

#include "screen.h"


Screen::Screen(int w, int h, float fov, const char *title):
w(w), h(h), fov(fov) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    this->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            this->w,
            this->h,
            SDL_WINDOW_OPENGL
    );

    this->context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    std::cout << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::left << "OpenGL Vendor: " << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::left << "OpenGL Renderer: " << (char *)glGetString(GL_RENDERER) << std::endl;

    glViewport(0, 0, this->w, this->h);
    glEnable(GL_DEPTH_TEST);

    this->perspective = glm::perspective(glm::radians(this->fov), (float) this->w / this->h, 0.1f, 100.0f);
}

Screen::~Screen() {
    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Screen::resize() {}

void Screen::flip() {
    SDL_GL_SwapWindow(this->window);
}
