#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera/cameraOrbit.h"
#include "graphics/mesh.h"
#include "graphics/mesh_animated.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "input.h"
#include "resourceCache.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "font.h"
#include "graphics/model.h"
#include "tiny_gltf.h"
#include "display.h"
#include "graphics/lines.h"
#include "graphics/skybox.h"

int main() {
    Display::init();
    Input::init(Display::window);

    Shader postShader("shaders/copy.vert", "shaders/copy.frag");
    postShader.use();

    // Main shader program
    Shader shaderAnim("shaders/vertex-skinned.vert", "shaders/fragment.frag");
    shaderAnim.use();
    shaderAnim.setUniform("tex", 0);

    Shader shaderStatic("shaders/vertex.vert", "shaders/fragment.frag");
    shaderStatic.use();
    shaderStatic.setUniform("tex", 0);

    // Skybox shader
    Shader shaderSkybox("shaders/skybox.vert", "shaders/skybox.frag");
    shaderSkybox.use();
    shaderSkybox.setUniform("skybox", 0);

    Shader shaderLine("shaders/line.vert", "shaders/line.frag");

    CameraOrbit camera(85.f, Display::viewPort);

    std::vector<Line> axisData;
    axisData.push_back(Line{
        {0.f, 0.f, 0.f}, {1.f, 0.f, 0.f},
        {50.f, 0.f, 0.f}, {0.f, 0.f, 0.f}
    });
    axisData.push_back(Line{
            {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f},
            {0.f, 50.f, 0.f}, {0.f, 0.f, 0.f}
    });
    axisData.push_back(Line{
            {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f},
            {0.f, 0.f, 50.f}, {0.f, 0.f, 0.f}
    });
    Lines axis(axisData);

    Skybox skybox("cubemap");

    DirectionalLight dLight = DirectionalLight();
    shaderStatic.use();
    shaderStatic.setDirectionalLight(dLight);
    shaderAnim.use();
    shaderAnim.setDirectionalLight(dLight);

    ResourceCache<Texture> textureCache;
    ResourceCache<MeshAnimated> meshAnimCache;
    ResourceCache<Mesh> meshCache;

    Model one(meshAnimCache.loadResource("stg44"), textureCache.loadResource("stg44"));
    Model two(meshCache.loadResource("stg44"), textureCache.loadResource("stg44"));
    two.translate(1.f, 0.f, 0.f);

    //glLineWidth(100.f);

    while (!Display::shouldClose) {
        Display::update();

        if (Input::getKey(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(Display::window, true);

        camera.update();

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene
        shaderStatic.use();
        shaderStatic.setUniform("dLight.direction", camera.getView() * dLight.direction);
        shaderAnim.use();
        shaderAnim.setUniform("dLight.direction", camera.getView() * dLight.direction);

        one.rotate(
                (360.f * 1.f) * ((float) Display::timeDelta / 10.f),
                0.f, 1.f, 0.f,
                false
        );
        two.rotate(
            (360.f * 1.f) * ((float) Display::timeDelta / 10.f),
            0.f, 1.f, 0.f,
            false
        );

        skybox.draw(shaderSkybox,
                    glm::mat4(glm::mat3(camera.getView())),
                    camera.projection);

        if (two.mesh->meshType == MeshType::ANIMATED) {
            two.draw(shaderAnim, camera.projection, camera.getView());
        } else {
            two.draw(shaderStatic, camera.projection, camera.getView());
        }

        if (one.mesh->meshType == MeshType::ANIMATED) {
            one.draw(shaderAnim, camera.projection, camera.getView());
        } else {
            one.draw(shaderStatic, camera.projection, camera.getView());
        }

        axis.draw(shaderLine, camera.projection, camera.getView());


        Display::flip(postShader, 0);
        Input::update();
    }

    Display::destroy();

    return 0;
}
