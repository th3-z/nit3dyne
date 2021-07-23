#include <iostream>
#include <random>
#include <iomanip>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#include "camera/cameraOrbit.h"
#include "camera/cameraFree.h"
#include "graphics/mesh.h"
#include "graphics/mesh_animated.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "input.h"
#include "resourceCache.h"
#include "display.h"
#include "graphics/lines.h"
#include "graphics/skybox.h"
#include "graphics/model.h"
#include "graphics/terrain.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"



int main() {
    Display::init();
    Input::init(Display::window);

    Shader postShader("shaders/post.vert", "shaders/post.frag");
    postShader.use();
    postShader.setUniform("tex", 0);
    postShader.setUniform("texDither", 1);
    //Texture textureDither("dith");

    // Main shader program
    Shader shaderAnim("shaders/vertex-skinned.vert", "shaders/fragment.frag");
    shaderAnim.use();
    shaderAnim.setUniform("tex", 0);

    Shader shaderStatic("shaders/vertex.vert", "shaders/fragment.frag");
    shaderStatic.use();
    shaderStatic.setUniform("tex", 0);

    Shader shaderTerrain("shaders/terrain.vert", "shaders/terrain.frag");
    shaderTerrain.use();
    shaderTerrain.setUniform("tex", 0);

    Shader shaderNormals("shaders/normals.vert", "shaders/normals.frag", "shaders/normals.geo");

    // Skybox shader
    Shader shaderSkybox("shaders/skybox.vert", "shaders/skybox.frag");
    shaderSkybox.use();
    shaderSkybox.setUniform("skybox", 0);

    Shader shaderLine("shaders/line.vert", "shaders/line.frag");

    CameraFree camera(85.f, Display::viewPort);

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

    Skybox skybox("test");

    DirectionalLight dLight = DirectionalLight();
    dLight.diffuse = glm::vec3(0.8, 0.8, 0.8);
    dLight.ambient = glm::vec3(0.1, 0.1, 0.1);
    shaderStatic.use();
    shaderStatic.setDirectionalLight(dLight);
    shaderAnim.use();
    shaderAnim.setDirectionalLight(dLight);
    shaderTerrain.use();
    shaderTerrain.setDirectionalLight(dLight);

    ResourceCache<Texture> textureCache;
    ResourceCache<MeshAnimated> meshAnimCache;
    ResourceCache<Mesh> meshCache;

    Model one(meshAnimCache.loadResource("hand"), textureCache.loadResource("error"));
    Model two(meshAnimCache.loadResource("stg44"), textureCache.loadResource("stg44"));
    two.translate(0.f, 0.f, 5.f);
//    two.scale(.25f, .25f, .25f);
//    one.scale(.04556f, .04556f, .04556f);
    Terrain test = Terrain("ny40");
    std::shared_ptr<Texture> testT = textureCache.loadResource("ny40");

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
        shaderTerrain.use();
        shaderTerrain.setUniform("dLight.direction", camera.getView() * dLight.direction);

        two.rotate(
                (360.f * 1.f) * ((float) Display::timeDelta / 10.f),
                0.f, 1.f, 0.f,
                false
        );

        skybox.draw(shaderSkybox,
                    glm::mat4(glm::mat3(camera.getView())),
                    camera.projection);

        if (one.mesh->meshType == MeshType::ANIMATED) {
            one.draw(shaderAnim, camera.projection, camera.getView());
        } else {
            one.draw(shaderStatic, camera.projection, camera.getView());
        }

        axis.draw(shaderLine, camera.projection, camera.getView());


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, testT->handle);
        test.draw(shaderTerrain, camera.projection, camera.getView());
        two.draw(shaderAnim, camera.projection, camera.getView());

        if (Input::getKey(GLFW_KEY_TAB)) {
            shaderNormals.use();
            shaderNormals.setUniform("model", test.model);
            shaderNormals.setUniform("view", camera.getView());
            shaderNormals.setUniform("projection", camera.projection);
            test.draw(shaderNormals, camera.projection, camera.getView());
        }

        Display::flip(postShader);
        Input::update();
    }

    Display::destroy();

    return 0;
}
