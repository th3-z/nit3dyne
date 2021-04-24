#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "shader.h"

float vertices[] = {
        0.5f,  0.5f, 0.0f,  // 0 - top right
        0.5f, -0.5f, 0.0f,  // 1 - bottom right
        -0.5f, -0.5f, 0.0f, // 2 - bottom left
        -0.5f,  0.5f, 0.0f  // 3 - top left
};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};


int main() {
    // INIT
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    std::cout << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::left << "OpenGL Vendor:" << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::left << "OpenGL Renderer:" << (char *)glGetString(GL_RENDERER) << std::endl;

    glViewport(0, 0, 640, 480);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    SDL_GL_SetSwapInterval(1);  // Wait for vblank on swap

    // SHADERS

    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    shader.setFloat("someFloat", 1.0f);
    shader.use();


    // Create new VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Bind VAO, VAO will remember state set in step 2/3, and which VBO to use
    glBindVertexArray(VAO);
    // Copy vertices array into a new VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Copy vertex indicies into a new EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SDL_Event event = { 0 };
    bool should_quit = false;

    while (!should_quit) {
        // Events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            should_quit = true;
                            break;
                        case SDLK_1:
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            break;
                        case SDLK_2:
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            break;
                    }
                    break;
            }
        }

        // Clear
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Render...
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Flip buffer
        SDL_GL_SwapWindow(window);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/*
 * Requirements:
 *   libsdl2-dev
 *   g++
 *   cmake
 *
 */