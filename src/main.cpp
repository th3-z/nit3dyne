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

#include "shader.h"
#include "texture.h"
#include "camera/cameraFps.h"
#include "camera/cameraFree.h"
#include "mesh.h"
#include "screen.h"
#include "input.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include "font.h"

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

    Font font("The quick brown fox jumps over the lazy dog.");
    //font.draw();

    // SHADERS
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
    shader.use();
    shader.setInt("tex", 0);

    // TODO: Move into screen.cpp
    Shader postShader("shaders/post.vert", "shaders/post.frag");
    postShader.use();
    postShader.setInt("tex", 0);
    postShader.setInt("texDither", 1);

    std::string textureType("diffuse");
    std::string texture0FilePath("res/textures/0.png");
    Texture texture0(textureType, texture0FilePath);
    std::string texture1FilePath("res/textures/1.png");
    Texture texture1(textureType, texture1FilePath);
    std::string texture2FilePath("res/textures/2.png");
    Texture texture2(textureType, texture2FilePath);

    // TODO: Move into screen.cpp
    std::string ditherFilePath("res/textures/dith.png");
    Texture textureDither(textureType, ditherFilePath);

    Mesh cube = Mesh("res/cube.glb");
    Mesh suzanne = Mesh("res/suzanne.glb");
    Mesh sphere= Mesh("res/sphere.glb");

    glm::vec4 sunPosition = glm::vec4(5.0, 5.0, 0.0, 1.0);
    glm::vec3 sunColor = glm::vec3(0.65, 0.8, 0.9);

    double timeDelta, timeNow, timeLast;
    unsigned long frames = 0;

    SoLoud::Soloud soloud; // Engine core
    SoLoud::Wav sample;    // One sample

// Initialize SoLoud (automatic back-end selection)
    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
    soloud.setGlobalVolume(4.f);

    sample.load("res/sound/dunkelheit.ogg"); // Load a wave file
    sample.setLooping(true);
    sample.set3dMinMaxDistance(1, 30);
    sample.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);

    SoLoud::handle sampleHandle = soloud.play3d(sample, 0.f, 0.f, 0.f);        // Play it

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

        shader.use();

        // Clear
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sun
        glm::vec3 viewSunPos = windowState.camera->getView() * (glm::mat4(1.0f) * sunPosition);
        shader.setVec3("sunPosition", viewSunPos);
        shader.setVec3("sunColor", sunColor);

        // Suzanne
        glActiveTexture(GL_TEXTURE0);  // Active texture unit
        glBindTexture(GL_TEXTURE_2D, texture0.handle);  // Bind texture

        glm::mat4 model = glm::mat4(1.0f);
        float rotation = (int) (glfwGetTime()*100) % 360;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.f, 0.1f));
        model = glm::translate(model, glm::vec3(0.f, 0.5f, -2.5f));

        glm::mat4 mvp = screen.perspective * windowState.camera->getView() * model;
        shader.setMat4("mvp", mvp);

        glm::mat4 modelView = windowState.camera->getView() * model;
        shader.setMat4("modelView", modelView);

        glm::mat3 normalMat = glm::inverse(
            glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        suzanne.draw();

        glm::vec4 suzannePosView =  modelView * glm::vec4(0.5,0.5,0.5,1.0);
        float w = suzannePosView.w;
        soloud.set3dSourceParameters(sampleHandle, suzannePosView.x/w, suzannePosView.y/w, suzannePosView.z/w, 0.f, 0.f, 0.f);

        soloud.set3dListenerPosition(0,0,0);

        soloud.update3dAudio();

        // Cube
        glBindTexture(GL_TEXTURE_2D, texture1.handle);  // Bind texture

        model = glm::mat4(1.0f);

        mvp = screen.perspective * windowState.camera->getView() * model;
        shader.setMat4("mvp", mvp);

        modelView = windowState.camera->getView() * model;
        shader.setMat4("modelView", modelView);

        normalMat = glm::inverse(
                glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        cube.draw();

        // Sphere
        glBindTexture(GL_TEXTURE_2D, texture2.handle);  // Bind texture

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 5.f, 6.5f));
        rotation = (int) (glfwGetTime()*100) % 360;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));

        mvp = screen.perspective * windowState.camera->getView() * model;
        shader.setMat4("mvp", mvp);

        modelView = windowState.camera->getView() * model;
        shader.setMat4("modelView", modelView);

        normalMat = glm::inverse(
                glm::transpose(glm::mat3(modelView))
        );
        shader.setMat3("normalMat", normalMat);

        sphere.draw();

        font.draw();

        screen.flip(postShader, textureDither.handle);
    }

    delete windowState.camera;

    return 0;
}
