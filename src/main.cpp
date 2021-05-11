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
#include "screen.h"
#include "input.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

const unsigned int SCREEN_H = 486;
const unsigned int SCREEN_W = 864;
const float SCREEN_FOV = 45.f;

int main() {
    Screen screen(SCREEN_W, SCREEN_H, SCREEN_FOV, "Pain");

    // SHADERS
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();

    Camera *camera = new CameraFree;
    Input input(&camera);

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


    float timeDelta = 0.f;
    float timeLast = 0.f;

    while (!input.quit) {
        float timeCurrent = (float) SDL_GetTicks() / 1000;
        timeDelta = timeCurrent - timeLast;
        timeLast = timeCurrent;

        input.handleEvents(timeDelta);

        // Clear
        glClearColor(0.9f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sun
        glm::vec3 viewSunPos = camera->getView() * (glm::mat4(1.0f) * sunPosition);
        shader.setVec3("sunPosition", viewSunPos);
        shader.setVec3("sunColor", sunColor);

        // Suzanne
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture0.handle);  // Bind texture
        shader.setInt("tex", 0);

        glm::mat4 model = glm::mat4(1.0f);
        float rotation = (SDL_GetTicks() % 3600) / 10;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.f, 0.1f));
        model = glm::translate(model, glm::vec3(0.f, 0.5f, -2.5f));

        glm::mat4 mvp = screen.perspective * camera->getView() * model;
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

        mvp = screen.perspective * camera->getView() * model;
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

        mvp = screen.perspective * camera->getView() * model;
        shader.setMat4("mvp", mvp);

        modelView = camera->getView() * model;
        shader.setMat4("modelView", modelView);

        normalMat = glm::inverse(
                glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        sphere.render(shader);

        screen.flip();
    }

    return 0;
}
