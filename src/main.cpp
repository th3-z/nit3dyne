#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <soloud/include/soloud.h>
#include <soloud/include/soloud_wav.h>
#include <entt/entt.hpp>
#include <memory>
#include <array>

#include "camera/cameraFps.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/terrain.h"
#include "input.h"
#include "graphics/skybox.h"
#include "resourceCache.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "font.h"
#include "graphics/model.h"
#include "tiny_gltf.h"
#include "graphics/lines.h"


int main() {
    Display::init();
    Input::init(Display::window);

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
    Font font("This text should not be upside down anymore!");

    // Main shader program
    Shader shaderAnim("shaders/vertex-skinned.vert", "shaders/fragment.frag");
    shaderAnim.use();
    shaderAnim.setUniform("tex", 0);

    Shader shaderUnlit("shaders/vertex.vert", "shaders/fragment-unlit.frag");
    shaderAnim.use();
    shaderAnim.setUniform("tex", 0);

    Shader shaderTerrain("shaders/terrain.vert", "shaders/terrain.frag");
    shaderTerrain.use();
    shaderTerrain.setUniform("tex", 0);

    Shader shaderNormals("shaders/normals.vert", "shaders/normals.frag", "shaders/normals.geo");

    Shader shaderStatic("shaders/vertex.vert", "shaders/fragment.frag");
    shaderStatic.use();
    shaderStatic.setUniform("tex", 0);

    Shader shaderLine("shaders/line.vert", "shaders/line.frag");

    CameraFree camera(85.f, Display::viewPort);

    // Scene
    DirectionalLight dLight = DirectionalLight();
    shaderStatic.use();
    shaderStatic.setDirectionalLight(dLight);
    shaderAnim.use();
    shaderAnim.setDirectionalLight(dLight);

    ResourceCache<Texture> textureCache;
    ResourceCache<MeshAnimated> meshAnimCache;
    ResourceCache<Mesh> meshCache;

    SpotLight sLight = SpotLight();
    shaderStatic.use();
    shaderStatic.setSpotLight(sLight);
    shaderAnim.use();
    shaderAnim.setSpotLight(sLight);

    Skybox skybox("test");
    Terrain test = Terrain("ny40");

    std::vector<Line> axisData;
    axisData.push_back(Line{
            {0.f, 0.f, 0.f}, {1.f, 0.f, 0.f},
            {500.f, 0.f, 0.f}, {0.f, 0.f, 0.f}
    });
    axisData.push_back(Line{
            {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f},
            {0.f, 500.f, 0.f}, {0.f, 0.f, 0.f}
    });
    axisData.push_back(Line{
            {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f},
            {0.f, 0.f, 500.f}, {0.f, 0.f, 0.f}
    });
    Lines axis(axisData);

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

    Model stg(meshAnimCache.loadResource("stg44"), textureCache.loadResource("stg44"));
    stg.translate(2.f, 2.f, 0.f);

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
    soloud.setGlobalVolume(.9f);

    sample.load("res/sound/avril14.ogg");
    sample.setLooping(true);
    sample.set3dMinMaxDistance(1, 30);
    sample.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
    SoLoud::handle sampleHandle = soloud.play3d(sample, 0.f, 0.f, 0.f);
    soloud.set3dSourceParameters(sampleHandle, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f);

    while (!Display::shouldClose) {
        Display::update();

        if (Input::getKey(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(Display::window, true);

        camera.update();

        // Clear FB
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        shaderStatic.use();
        shaderStatic.setUniform("dLight.direction", camera.getView() * dLight.direction);

        test.draw(shaderStatic, camera.projection, camera.getView());
        plane.draw(shaderStatic, camera.projection, camera.getView());
        sphere.draw(shaderStatic, camera.projection, camera.getView());

        for (auto &prop : props) {
            prop->rotate((360.f * 1.) * (Display::timeDelta / 10.), 0.f, 1.f, 0.f, false);
            prop->draw(shaderStatic, camera.projection, camera.getView());
        }

        // FIXME: Lighting calculations still need the real camera view mat
        viewModel.draw(shaderUnlit, glm::mat4(1.f), camera.projection);

        // Update audio
        glm::vec4 sourcePosView =  (camera.getView() * glm::mat4(1.f)) * glm::vec4(0.f,0.f,0.f,1.f);
        float w = sourcePosView.w;
        soloud.set3dSourceParameters(sampleHandle, sourcePosView.x/w, sourcePosView.y/w, sourcePosView.z/w, 0.f, 0.f, 0.f);

        soloud.set3dListenerPosition(0,0,0);
        soloud.update3dAudio();

        // Render skybox
        skybox.draw(shaderSkybox,
                    glm::mat4(glm::mat3(camera.getView())),
                    camera.projection);

        axis.draw(shaderLine, camera.projection, camera.getView());

        // Render UI
        font.draw();

        // Animated mesh
        stg.draw(shaderAnim, camera.projection, camera.getView());

        Display::flip(postShader, textureDither.handle);
        Input::update();
    }

    Display::destroy();

    return 0;
}
