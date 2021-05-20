#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <soloud/include/soloud.h>
#include <soloud/include/soloud_wav.h>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "camera/cameraFps.h"
#include "graphics/mesh.h"
#include "screen.h"
#include "input.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include "font.h"
#include "graphics/model.h"

#ifndef NDEBUG
const unsigned int SCREEN_W = 864;
const unsigned int SCREEN_H = 486;
#else
const unsigned int SCREEN_W = 1920;
const unsigned int SCREEN_H = 1200;
#endif
const float SCREEN_FOV = 90.f;

const unsigned int TARGET_FPS = 75;
const double TARGET_FRAMETIME = 1.0 / TARGET_FPS;

int main() {
    Screen screen(SCREEN_W, SCREEN_H, SCREEN_FOV, "Pain");
    WindowState windowState;
    windowState.camera = new CameraFps;
    glfwSetWindowUserPointer(screen.window, (void*) &windowState);
    Input::registerCallbacks(screen.window);

    // TODO: Move into screen.cpp
    Shader postShader("shaders/post.vert", "shaders/post.frag");
    postShader.use();
    postShader.setUniform("tex", 0);
    postShader.setUniform("texDither", 1);
    std::string ditherFilePath("res/textures/dith.png");
    Texture textureDither(ditherFilePath);

    // Test font renderer
    Font font("The quick brown fox jumps over the lazy dog.");

    // Main shader program
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();
    shader.setUniform("tex", 0);

    // Scene
    DirectionalLight dLight = DirectionalLight();
    shader.setDirectionalLight(dLight);

    SpotLight sLight = SpotLight();
    shader.setSpotLight(sLight);

    Model suzanne = Model("res/suzanne.glb", "res/textures/0.png");
    suzanne.setMaterial(Materials::metallic);
    suzanne.translate(0.f, 2.0f, 5.f);

    std::vector<Model*> monkeys;
    int nMonkeys = 128;
    for (int i = 0; i < nMonkeys; ++i) {
        monkeys.emplace_back(
            new Model("res/suzanne.glb", "res/textures/0.png")
        );
        monkeys[i]->setMaterial(Materials::metallic);
        monkeys[i]->translate((i * 2.5f) - (2.5f*nMonkeys)/2, sin(i)*5 + 6.5f, -5.f);
    }

    Model cube = Model("res/cube.glb", "res/textures/1.png");
    cube.setMaterial(Materials::metallic);
    cube.translate(5.f, 2.f, 0.f);

    Model plane = Model("res/plane.glb", "res/textures/1.png");
    plane.setMaterial(Materials::metallic);
    plane.scale(7.f, 0.f, 7.f);
    //plane.translate(-7.5f, 0.f, -7.5f);

    Model sphere = Model("res/sphere.glb", "res/textures/3.png");
    sphere.translate(0.f, 5.f, 0.f);
    sphere.scale(1.f, 1.f, 1.f);

    // Audio
    SoLoud::Soloud soloud; // Engine core
    SoLoud::Wav sample;    // One sample

    // Initialize SoLoud, miniaudio backend
    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::BACKENDS::MINIAUDIO);
    soloud.setGlobalVolume(.5f);

    sample.load("res/sound/dunkelheit.ogg");
    sample.setLooping(true);
    sample.set3dMinMaxDistance(1, 30);
    sample.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
    SoLoud::handle sampleHandle = soloud.play3d(sample, 0.f, 0.f, 0.f);
    soloud.set3dSourceParameters(
        sampleHandle,
        0.f, 5.f, 0.f,
        0.f, 0.f, 0.f
    );

    // Timing
    double timeNow, timeLast = 0.;
    unsigned long frames = 0;

    while (!glfwWindowShouldClose(screen.window)) {
        // Calculate frame time, cap framerate to target
        timeLast = timeNow;
        timeNow = glfwGetTime();
        while ((timeNow - timeLast) < TARGET_FRAMETIME) timeNow = glfwGetTime();
        windowState.timeDelta = timeNow - timeLast;

        if (frames % (TARGET_FPS*5) == 0)
            std::cout << "Previous frame time: "
                      << windowState.timeDelta*1e3 << "ms\t("
                      << 1/windowState.timeDelta << "fps)"
                      << std::endl;

        // Handle continuous response keys
        Input::processContinuousInput(screen.window);

        // Clear FB
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        shader.use();
        shader.setUniform("dLight.direction", windowState.camera->getView() * dLight.direction);

        cube.draw(shader, screen.perspective, windowState.camera->getView());

        plane.draw(shader, screen.perspective, windowState.camera->getView());

        sphere.rotate(
            (360.f * .1 /* rev per s */) * windowState.timeDelta, 0.f, 1.f, 0.f, false
        );
        sphere.draw(shader, screen.perspective, windowState.camera->getView());

        suzanne.draw(shader, screen.perspective, windowState.camera->getView());

        for (Model *monkey : monkeys) {
            monkey->rotate(
                (360.f * 1.) * windowState.timeDelta, 0.f, 1.f, 0.f, false
            );
            monkey->draw(shader, screen.perspective, windowState.camera->getView());
        }


        // Update audio
        soloud.set3dListenerPosition(windowState.camera->position.x,windowState.camera->position.y,windowState.camera->position.z);
        soloud.update3dAudio();

        // Render UI
        font.draw();

        screen.flip(postShader, textureDither.handle);
        frames++;
    }

    for (Model *monkey : monkeys) {
        delete monkey;
    }
    delete windowState.camera;

    return 0;
}
