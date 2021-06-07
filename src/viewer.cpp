#include <chrono>
#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soloud/include/soloud.h>
#include <soloud/include/soloud_wav.h>
#include <entt/entt.hpp>
#include <memory>
#include <array>

#include "camera/cameraOrbit.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "input.h"
#include "screen.h"
#include "graphics/skybox.h"
#include "resourceCache.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "font.h"
#include "graphics/model.h"
#include "tiny_gltf.h"

const int SCREEN_W = 800;
const int SCREEN_H = 600;

const float SCREEN_FOV = 70.f;

const unsigned int TARGET_FPS = 75;
const double TARGET_FRAMETIME = 1.0 / TARGET_FPS;

int main() {
    std::pair<int, int> viewPort(SCREEN_W, SCREEN_H);
    Screen screen(viewPort, viewPort, "Viewer");

    WindowState windowState;
    windowState.camera = std::make_unique<CameraOrbit>(SCREEN_FOV, viewPort);
    glfwSetWindowUserPointer(screen.window, (void *) &windowState);
    Input::registerCallbacks(screen.window);

    Shader postShader("shaders/copy.vert", "shaders/copy.frag");
    postShader.use();

    // Main shader program
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();
    shader.setUniform("tex", 0);

    // Scene
    DirectionalLight dLight = DirectionalLight();
    shader.setDirectionalLight(dLight);

    ResourceCache<Texture> textureCache;
    ResourceCache<Mesh> meshCache;
    Model m4a1(meshCache.loadResource("m4a1"), textureCache.loadResource("m4a1"));

    // Timing
    double timeNow, timeLast = 0.;

    while (!glfwWindowShouldClose(screen.window)) {
        // Calculate frame time, cap framerate to target
        timeLast = timeNow;
        timeNow = glfwGetTime();
        while ((timeNow - timeLast) < TARGET_FRAMETIME)
            timeNow = glfwGetTime();
        windowState.timeDelta = timeNow - timeLast;

        // Clear FB
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        shader.use();
        shader.setUniform("dLight.direction", windowState.camera->getView() * dLight.direction);

        m4a1.draw(shader, windowState.camera->projection, windowState.camera->getView());

        screen.flip(postShader, 0);
    }

    return 0;
}
