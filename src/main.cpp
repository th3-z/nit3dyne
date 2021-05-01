#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"

float vertices[] = {
        // Verts              // Colors           // Tex cords
        0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // 0 - top right
        0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // 1 - bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // 2 - bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f   // 3 - top left
};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};


int main() {
    glm::mat4 trans = glm::mat4(1.0f);  // New identity matrix
    trans = glm::rotate(  //  Rotate matrix
        trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)
    );
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  // Scale matrix

    // INIT
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

    // SHADERS

    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SDL_Event event = { 0 };
    bool should_quit = false;

    std::string texture0FilePath("res/textures/0.png");
    Texture texture0(texture0FilePath);

    std::string texture1FilePath("res/textures/1.png");
    Texture texture1(texture1FilePath);

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

        float hue = sin((SDL_GetTicks()%75)) / 2.0f + 0.5f;
        trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float) (SDL_GetTicks()%75)/1300, glm::vec3(1.0f, 1.0f, 0.2f));
        trans = glm::translate(trans, glm::vec3((float) (SDL_GetTicks()%75)/1300,(float) (SDL_GetTicks()%75)/1300, (float) (SDL_GetTicks()%75)/1300));

        shader.setFloat("colorHue", hue);
        shader.setMat4("transformation", trans);

        // Clear
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Render...
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture0.id);  // Bind texture
        shader.setInt("tex0", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1.id);
        shader.setInt("tex1", 1);

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
