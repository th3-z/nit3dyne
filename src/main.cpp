#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"
#include "camera/cameraFixed.h"
#include "camera/cameraFree.h"
#include "model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

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
            800,
            600,
            SDL_WINDOW_OPENGL
    );

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    std::cout << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::left << "OpenGL Vendor: " << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::left << "OpenGL Renderer: " << (char *)glGetString(GL_RENDERER) << std::endl;

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);  // Enable depth test

    return Context{window, context};
}

int main() {
    Context context = initGl();

    // SHADERS
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();

    Camera *camera;
    CameraFree cameraFree;
    CameraFixed cameraFixed;
    camera = &cameraFree;
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.0f);

    std::string textureType("diffuse");
    std::string texture0FilePath("res/textures/0.png");
    Texture texture0(textureType, texture0FilePath);
    std::string texture1FilePath("res/textures/1.png");
    Texture texture1(textureType, texture1FilePath);
    std::string texture2FilePath("res/textures/2.png");
    Texture texture2(textureType, texture2FilePath);

    Model cube = Model("res/cube.glb");
    Model suzanne = Model("res/suzanne.glb");
    Model sphere= Model("res/sphere.glb");

    glm::vec4 sunPosition = glm::vec4(5.0, 5.0, 0.0, 1.0);
    glm::vec3 sunColor = glm::vec3(1.0, 0.7, 0.5);

    SDL_Event event = {0};
    bool should_quit = false;
    float timeDelta = 0.f;
    float timeLast = 0.f;

    while (!should_quit) {
        float timeCurrent = (float) SDL_GetTicks() / 1000;
        timeDelta = timeCurrent - timeLast;
        timeLast = timeCurrent;

        // Events
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
                        case SDLK_f:
                            camera = &cameraFixed;
                            break;
                        case SDLK_g:
                            camera = &cameraFree;
                            break;
                    }
                    break;
            }
        }

        camera->handleDirection(directions, timeDelta);
        camera->handleMouse(mX, mY);

        // Clear
        glClearColor(0.9f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sun
        glm::vec3 viewSunPos = camera->getView() * (glm::mat4(1.0f) * sunPosition);
        //std::cout << "x" << viewSunPos.x << " y" << viewSunPos.y<< " z" << viewSunPos.z<< std::endl;
        shader.setVec3("sunPosition", viewSunPos);
        shader.setVec3("sunColor", sunColor);

        // Suzanne
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture0.handle);  // Bind texture

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 0.5f, -2.5f));
        float rotation = (SDL_GetTicks() % 3600) / 10;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.f, 0.1f));

        glm::mat4 mvp = projection * camera->getView() * model;
        shader.setMat4("mvp", mvp);

        glm::mat4 modelView = camera->getView() * model;
        shader.setMat4("modelView", modelView);

        glm::mat3 normalMat = glm::inverse(
            glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        suzanne.render(shader);

        // Cube
        glBindTexture(GL_TEXTURE_2D, texture1.handle);  // Bind texture
        shader.setInt("tex", 0);

        model = glm::mat4(1.0f);
        mvp = projection * camera->getView() * model;
        shader.setMat4("mvp", mvp);

        modelView = camera->getView() * model;
        shader.setMat4("modelView", modelView);

        normalMat = glm::inverse(
                glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        cube.render(shader);

        // Sphere
        glBindTexture(GL_TEXTURE_2D, texture2.handle);  // Bind texture
        shader.setInt("tex", 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 5.f, 6.5f));
        rotation = (SDL_GetTicks() % 360);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));

        mvp = projection * camera->getView() * model;
        shader.setMat4("mvp", mvp);

        modelView = camera->getView() * model;
        shader.setMat4("modelView", modelView);

        normalMat = glm::inverse(
                glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        sphere.render(shader);

        // Flip buffer
        SDL_GL_SwapWindow(context.window);
    }

    SDL_GL_DeleteContext(context.context);
    SDL_DestroyWindow(context.window);
    SDL_Quit();

    return 0;
}
