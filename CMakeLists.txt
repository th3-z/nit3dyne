cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} lib/glad/include lib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(gl
    lib/glad/src/glad.c
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
target_link_libraries(gl ${SDL2_LIBRARIES} ${GLM_LIBRARIES} ${CMAKE_DL_LIBS})

add_custom_command(
        TARGET gl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/src/res ${CMAKE_CURRENT_BINARY_DIR}/res
        COMMENT "Linking resources"
)
