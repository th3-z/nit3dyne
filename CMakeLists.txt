cmake_minimum_required(VERSION 3.12.4)
project(nit3dyne)

add_subdirectory(external)

set(SOURCES
    nit3dyne/graphics/shader.cpp nit3dyne/graphics/shader.h
    nit3dyne/graphics/texture.cpp nit3dyne/graphics/texture.h
    nit3dyne/graphics/mesh.cpp nit3dyne/graphics/mesh.h
    nit3dyne/graphics/mesh_animated.cpp nit3dyne/graphics/mesh_animated.h
    nit3dyne/graphics/model.cpp nit3dyne/graphics/model.h
    nit3dyne/graphics/material.cpp nit3dyne/graphics/material.h
    nit3dyne/graphics/lighting.h
    nit3dyne/graphics/skybox.cpp nit3dyne/graphics/skybox.h
    nit3dyne/graphics/lines.cpp nit3dyne/graphics/lines.h
    nit3dyne/graphics/terrain.cpp nit3dyne/graphics/terrain.h

    nit3dyne/camera/camera.cpp nit3dyne/camera/camera.h
    nit3dyne/camera/cameraFps.cpp nit3dyne/camera/cameraFps.h
    nit3dyne/camera/cameraFree.cpp nit3dyne/camera/cameraFree.h
    nit3dyne/camera/cameraOrbit.cpp nit3dyne/camera/cameraOrbit.h

    nit3dyne/animation/joint.cpp nit3dyne/animation/joint.h
    nit3dyne/animation/animation.cpp nit3dyne/animation/animation.h
    nit3dyne/animation/sampler.h
    nit3dyne/animation/animator.cpp nit3dyne/animation/animator.h
    nit3dyne/animation/skin.cpp nit3dyne/animation/skin.h

    nit3dyne/utils/gltf_utils.cpp nit3dyne/utils/gltf_utils.h
    nit3dyne/utils/rand.h

    nit3dyne/display.cpp nit3dyne/display.h
    nit3dyne/input.cpp nit3dyne/input.h
    nit3dyne/font.cpp nit3dyne/font.h
    nit3dyne/resourceCache.h
        nit3dyne/graphics/billboard.cpp nit3dyne/graphics/billboard.h)

add_library(nit3dyne STATIC ${SOURCES})
target_compile_options(nit3dyne PRIVATE "-Wall")
target_include_directories(nit3dyne PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(nit3dyne PUBLIC glfw glm glad soloud EnTT json tiny_gltf stb ${CMAKE_DL_LIBS})

# Copy engine resources and shaders
add_custom_command(TARGET nit3dyne POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/nit3dyne/res
    ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_custom_command(TARGET nit3dyne POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/nit3dyne/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
)