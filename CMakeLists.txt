cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
  "-O3"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} lib/glad/include)

add_library(glad lib/glad/src/glad.c)

add_executable(gl src/main.cpp src/shader.cpp src/shader.h src/texture.cpp src/texture.h src/util/image.cpp src/util/image.h src/camera.cpp src/camera.h src/cube.h)
target_link_libraries(gl glad ${SDL2_LIBRARIES} ${GLM_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${CMAKE_DL_LIBS})

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
