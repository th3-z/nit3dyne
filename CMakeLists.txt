cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)

# Header-only libs
include_directories(lib)

# External targets
set(GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(lib/glad)
set(SOLOUD_STATIC ON)
set(SOLOUD_BACKEND_MINIAUDIO ON)
set(SOLOUD_BACKEND_SDL2 OFF)
set(SOLOUD_BACKEND_NULL OFF)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_WINMM OFF)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_OPENSLES ON)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_COREAUDIO OFF)
add_subdirectory(lib/soloud)

add_executable(gl
    src/main.cpp
        src/graphics/shader.cpp src/graphics/shader.h src/graphics/texture.cpp src/graphics/texture.h
        src/graphics/mesh.cpp src/graphics/mesh.h
    src/camera/cameraFps.cpp src/camera/cameraFps.h
    src/camera/cameraFree.cpp src/camera/cameraFree.h
    src/camera/camera.cpp src/camera/camera.h
    src/screen.cpp src/screen.h
    src/input.cpp src/input.h
    src/font.cpp src/font.h
        src/graphics/model.cpp src/graphics/model.h src/graphics/material.cpp src/graphics/material.h src/graphics/lighting.cpp src/graphics/lighting.h src/graphics/skybox.cpp src/graphics/skybox.h)
target_link_libraries(gl glfw glm glad soloud ${CMAKE_DL_LIBS})

# Link resources
add_custom_command(
    TARGET gl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/src/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Linking resources"
)
