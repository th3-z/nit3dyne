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

#include "camera/cameraFps.h"
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

#ifndef NDEBUG
const int SCREEN_W = 1600;
const int SCREEN_H = 1000;
#else
const unsigned int SCREEN_W = 1920;
const unsigned int SCREEN_H = 1200;
#endif
const float SCREEN_FOV = 90.f;
const int SCREEN_W_VIRTUAL = 864;
const int SCREEN_H_VIRTUAL = 486;

const unsigned int TARGET_FPS = 75;
const double TARGET_FRAMETIME = 1.0 / TARGET_FPS;

int main() {
    std::pair<int, int> viewPort(SCREEN_W, SCREEN_H);
    std::pair<int, int> viewPortVirtual(SCREEN_W_VIRTUAL, SCREEN_H_VIRTUAL);
    Screen screen(viewPort, viewPortVirtual, "Pain");

    WindowState windowState;
    windowState.camera = std::make_unique<CameraFps>(SCREEN_FOV, viewPortVirtual);
    glfwSetWindowUserPointer(screen.window, (void *) &windowState);
    Input::registerCallbacks(screen.window);

    // TODO: Move into screen.cpp
    Shader postShader("shaders/post.vert", "shaders/post.frag");
    postShader.use();
    postShader.setUniform("tex", 0);
    postShader.setUniform("texDither", 1);
    Texture textureDither("dith");

    // Skybox shader
    Shader shaderSkybox("shaders/skybox.vert", "shaders/skybox.frag");
    shaderSkybox.use();
    shaderSkybox.setUniform("skybox", 0);

    // Test font renderer
    Font font("The quick brown fox jumps over the lazy dog.");

    // Main shader program
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();
    shader.setUniform("tex", 0);

    // Scene
    DirectionalLight dLight = DirectionalLight();
    shader.setDirectionalLight(dLight);

    ResourceCache<Texture> textureCache;
    ResourceCache<Mesh> meshCache;

    SpotLight sLight = SpotLight();
    shader.setSpotLight(sLight);

    Skybox skybox("cubemap");

    std::array<std::string, 8> propNames = {
        "desk", "chair", "axe", "speaker", "switch", "stg44", "m4a1", "cube"};
    std::vector<std::unique_ptr<Model>> props;

    for (size_t i = 0; i < propNames.size(); ++i) {
        props.emplace_back(std::make_unique<Model>(
            Model(meshCache.loadResource(propNames[i]), textureCache.loadResource(propNames[i]))));
        props[i]->translate(i * 5.f, 2.f, -2.f);
    }

    Model plane(meshCache.loadResource("plane"), textureCache.loadResource("red"));
    plane.setMaterial(Materials::metallic);
    plane.scale(7.f, 0.f, 7.f);

    Model sphere(meshCache.loadResource("sphere"), textureCache.loadResource("red"));
    sphere.translate(0.f, 5.f, 0.f);
    sphere.scale(1.f, 1.f, 1.f);

    Model viewModel(meshCache.loadResource("m4a1"), textureCache.loadResource("m4a1"));
    viewModel.translate(0.45f, -0.25f, -0.65f);
    viewModel.rotate(95.f, 0.f, 1.f, 0.f);
    viewModel.rotate(-5.f, 0.f, 0.f, 1.f);
    viewModel.rotate(-5.f, 0.f, 1.f, 0.f);
    viewModel.scale(.45f, .45f, .45f);

    // Audio
    SoLoud::Soloud soloud; // Engine core
    SoLoud::Wav sample;    // One sample

    // Initialize SoLoud, miniaudio backend
    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION,
                SoLoud::Soloud::BACKENDS::MINIAUDIO);
    soloud.setGlobalVolume(.5f);

    sample.load("res/sound/dunkelheit.ogg");
    sample.setLooping(true);
    sample.set3dMinMaxDistance(1, 30);
    sample.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
    SoLoud::handle sampleHandle = soloud.play3d(sample, 0.f, 0.f, 0.f);
    soloud.set3dSourceParameters(sampleHandle, 5.f, 2.f, 0.f, 0.05f, 0.f, 0.f);

    // Timing
    double timeNow, timeLast = 0.;
    unsigned long frames = 0;

    while (!glfwWindowShouldClose(screen.window)) {
        // Calculate frame time, cap framerate to target
        timeLast = timeNow;
        timeNow = glfwGetTime();
        while ((timeNow - timeLast) < TARGET_FRAMETIME)
            timeNow = glfwGetTime();
        windowState.timeDelta = timeNow - timeLast;

        if (frames % (TARGET_FPS * 5) == 0)
            std::cout << "Previous frame time: " << windowState.timeDelta * 1e3 << "ms\t("
                      << 1 / windowState.timeDelta << "fps)" << std::endl;

        // Handle continuous response keys
        Input::processContinuousInput(screen.window);

        // Clear FB
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        shader.use();
        shader.setUniform("dLight.direction", windowState.camera->getView() * dLight.direction);

        plane.draw(shader, windowState.camera->projection, windowState.camera->getView());
        sphere.draw(shader, windowState.camera->projection, windowState.camera->getView());

        for (auto &prop : props) {
            prop->rotate((360.f * 1.) * (windowState.timeDelta / 10.), 0.f, 1.f, 0.f, false);
            prop->draw(shader, windowState.camera->projection, windowState.camera->getView());
        }

        // FIXME: Lighting calculations still need the real camera view mat
        viewModel.draw(shader, glm::mat4(1.f), windowState.camera->projection);

        // Update audio
        soloud.set3dListenerPosition(
            windowState.camera->position.x, windowState.camera->position.y, windowState.camera->position.z);
        soloud.update3dAudio();

        // Render skybox
        skybox.draw(shaderSkybox,
                    glm::mat4(glm::mat3(windowState.camera->getView())),
                    windowState.camera->projection);

        // Render UI
        font.draw();

        screen.flip(postShader, textureDither.handle);
        frames++;
    }

    return 0;
}
