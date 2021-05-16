cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -Wall")

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
add_subdirectory(lib/soloud)

add_executable(gl
    src/main.cpp
    src/shader.cpp src/shader.h src/texture.cpp src/texture.h
    src/model.cpp src/model.h
    src/camera/cameraFixed.cpp src/camera/cameraFixed.h
    src/camera/cameraFree.cpp src/camera/cameraFree.h
    src/camera/camera.cpp src/camera/camera.h
    src/screen.cpp src/screen.h
    src/input.cpp src/input.h
    src/font.cpp src/font.h
)
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
