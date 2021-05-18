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

const unsigned int SCREEN_W = 1920;
const unsigned int SCREEN_H = 1200;
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

    // Assets
    Model suzanne = Model("res/suzanne.glb", "res/textures/0.png");
    suzanne.translate(0.f, 0.0f, 5.f);

    Model cube = Model("res/cube.glb", "res/textures/1.png");
    cube.translate(5.f, 0.f, 0.f);

    Model sphere = Model("res/sphere.glb", "res/textures/3.png");
    sphere.translate(0.f, 5.f, 0.f);
    sphere.scale(1.f, 0.5f, 3.f);

    // Sun
    glm::vec4 sunPosition = glm::vec4(5.0, 5.0, 0.0, 1.0);
    glm::vec3 sunColor = glm::vec3(0.65, 0.8, 0.9);
    shader.use();
    shader.setUniform("sunColor", sunColor);

    // Audio
    SoLoud::Soloud soloud; // Engine core
    SoLoud::Wav sample;    // One sample

    // Initialize SoLoud, miniaudio backend
    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::BACKENDS::MINIAUDIO);
    soloud.setGlobalVolume(4.f);

    sample.load("res/sound/dunkelheit.ogg"); // Load a wave file
    sample.setLooping(true);
    sample.set3dMinMaxDistance(1, 30);
    sample.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
    SoLoud::handle sampleHandle = soloud.play3d(sample, 0.f, 0.f, 0.f);        // Play it

    // Timing
    double timeDelta, timeNow, timeLast;
    unsigned long frames = 0;

    while (!glfwWindowShouldClose(screen.window)) {
        frames++;
        timeLast = timeNow;
        timeNow = glfwGetTime();
        timeDelta = timeNow - timeLast;
        if (frames % (TARGET_FPS*5) == 0)
            std::cout << "Frame time: " << timeDelta*1e3 << "ms\t(" << 1/timeDelta << "fps),\t";

        if (timeDelta < TARGET_FRAMETIME)
            std::this_thread::sleep_for(std::chrono::nanoseconds(
                    (int) ((TARGET_FRAMETIME - timeDelta) * 1.0e9 - 1.5e5)
            ));

        timeNow = glfwGetTime();
        timeDelta = timeNow - timeLast;
        windowState.timeDelta = timeDelta;
        if (frames % (TARGET_FPS*5) == 0)
            std::cout << timeDelta*1e3 << "ms\t(" << 1/timeDelta << "fps)" << std::endl;

        Input::processContinuousInput(screen.window);

        // Clear
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sun
        glm::vec3 viewSunPos = windowState.camera->getView() * (glm::mat4(1.0f) * sunPosition);
        shader.use();
        shader.setUniform("sunPosition", viewSunPos);

        // Cube
        cube.draw(shader, screen.perspective, windowState.camera->getView());

        // Sphere
        sphere.rotate(
            (360.f * .1 /* rev per s */) * timeDelta, 0.f, 1.f, 0.f, false
        );
        sphere.draw(shader, screen.perspective, windowState.camera->getView());

        // Suzanne
        suzanne.rotate(
            (360.f * 1.) * timeDelta, 1.f, 0.f, 0.f, false
        );
        suzanne.draw(shader, screen.perspective, windowState.camera->getView());


        // Audio test
//        glm::vec4 suzannePosView =  modelView * glm::vec4(0.5,0.5,0.5,1.0);
//        float w = suzannePosView.w;
//        soloud.set3dSourceParameters(
//            sampleHandle,
//            suzannePosView.x/w, suzannePosView.y/w, suzannePosView.z/w,
//            0.f, 0.f, 0.f
//        );
//
//        soloud.set3dListenerPosition(0,0,0);
//
//        soloud.update3dAudio();

        font.draw();

        screen.flip(postShader, textureDither.handle);
    }

    delete windowState.camera;

    return 0;
}
