#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

float verticesCube[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};

struct Context {
    SDL_Window *window;
    SDL_GLContext context;
};

Context initGl() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GL_SetSwapInterval(1);  // Wait for vblank on swap

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
    std::cout << std::left << "OpenGL Vendor: " << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::left << "OpenGL Renderer: " << (char *)glGetString(GL_RENDERER) << std::endl;

    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);  // Enable depth test

    return Context{window, context};
}

int main() {
    Context context = initGl();

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

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string textureType("diffuse");
    std::string texture0FilePath("res/textures/0.png");
    Texture texture(textureType, texture0FilePath);

    Camera camera = Camera();

    Model cube = Model("res/cube.glb");

    glm::vec3 sunPosition = glm::vec3(3.0, 10.0, -5.0);
    glm::vec3 sunColor = glm::vec3(1.0);


    SDL_Event event = {0};
    bool should_quit = false;

    while (!should_quit) {
        // Events
        int directions = 0;
        int mX = 0;
        int mY = 0;

        //continuous-response directions
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_W]) directions |= direction::FORWARD;
        if(keystate[SDL_SCANCODE_A]) directions |= direction::LEFT;
        if(keystate[SDL_SCANCODE_S]) directions |= direction::BACKWARD;
        if(keystate[SDL_SCANCODE_D]) directions |= direction::RIGHT;
        if(keystate[SDL_SCANCODE_SPACE]) directions |= direction::UP;
        if(keystate[SDL_SCANCODE_LSHIFT]) directions |= direction::DOWN;

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

        camera.move(directions, mX, mY);

        // Clear
        glClearColor(0.9f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sun
        shader.setVec3("sunPosition", sunPosition);
        shader.setVec3("sunColor", sunColor);

        // Render...
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture.handle);  // Bind texture
        shader.setInt("tex", 0);

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


        glm::mat4 model_mat = glm::mat4(1.0f);
        glm::mat4 model_rot = glm::mat4(1.0f);
        glm::vec3 model_pos = glm::vec3(-3, 0, -3);
        glm::mat4 trans =
                glm::translate(glm::mat4(1.0f), model_pos);  // reposition model
        model_rot = glm::rotate(model_rot, glm::radians(0.8f),
                                glm::vec3(0, 1, 0));  // rotate model on y axis
        model_mat = trans * model_rot;
        glm::mat4 view_mat = glm::lookAt(
                glm::vec3(2, 2, 20),                // Camera in World Space
                model_pos,             // and looks at the origin
                glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        shader.setMat4("model", model);

        cube.render(shader);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // Flip buffer
        SDL_GL_SwapWindow(context.window);
    }

    SDL_GL_DeleteContext(context.context);
    SDL_DestroyWindow(context.window);
    SDL_Quit();

    return 0;
}
