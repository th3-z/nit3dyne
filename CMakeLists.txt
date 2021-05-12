cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
  "-O3"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} lib/glad/include lib)
link_directories(lib)

add_library(glad lib/glad/src/glad.c)

add_executable(gl src/main.cpp src/shader.cpp src/shader.h src/texture.cpp src/texture.h src/model.cpp src/model.h src/camera/cameraFixed.cpp src/camera/cameraFixed.h src/camera/cameraFree.cpp src/camera/cameraFree.h src/camera/camera.cpp src/camera/camera.h src/screen.cpp src/screen.h src/input.cpp src/input.h)
target_link_libraries(gl glad ${SDL2_LIBRARIES} ${GLM_LIBRARIES} ${CMAKE_DL_LIBS})

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
