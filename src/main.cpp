#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "cube.h"


int main() {
    // INIT
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_CaptureMouse(SDL_TRUE);

    SDL_Window *window = SDL_CreateWindow(
        "Pain",
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
    glEnable(GL_DEPTH_TEST);  // Enable depth test

    // SHADERS
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();

    // Create new VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Bind VAO, VAO will remember state, and which VBO to use
    glBindVertexArray(VAO);
    // Copy vertices array into a new VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

    // Set vertex attributes pointers
    glVertexAttribPointer(
        0,  // Location
        3,  // Size
        GL_FLOAT,  // Type
        GL_FALSE,  // Normalized
        5 * sizeof(float),  // Stride
        (void*)0  // Start
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string texture0FilePath("res/textures/0.png");
    Texture texture0(texture0FilePath);

    std::string texture1FilePath("res/textures/1.png");
    Texture texture1(texture1FilePath);

    Camera camera = Camera();

    SDL_Event event = { 0 };
    bool should_quit = false;

    while (!should_quit) {
        // Events
        int keys = 0;
        int mX = 0;
        int mY = 0;

        //continuous-response keys
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_W]) keys |= key::UP;
        if(keystate[SDL_SCANCODE_A]) keys |= key::LEFT;
        if(keystate[SDL_SCANCODE_S]) keys |= key::DOWN;
        if(keystate[SDL_SCANCODE_D]) keys |= key::RIGHT;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    mX = event.motion.xrel;
                    mY = event.motion.yrel;
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

        camera.move(keys, mX, mY);

        // Clear
        glClearColor(0.9f, 0.5f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render...
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture0.id);  // Bind texture
        shader.setInt("tex0", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1.id);
        shader.setInt("tex1", 1);

        glm::mat4 model = glm::mat4(1.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", camera.view);
        shader.setMat4("projection", camera.projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(model, glm::vec3(0.f, 0.5f, -2.5f));
        float degsRot = (SDL_GetTicks()%3600)/10;
        model = glm::rotate(model, glm::radians(degsRot), glm::vec3(0.5f, 1.f, 0.1f));

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Flip buffer
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
