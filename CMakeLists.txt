cmake_minimum_required(VERSION 3.10)
project(gl)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} lib/glad/include)

add_library(glad lib/glad/src/glad.c)

add_executable(gl src/main.cpp src/shader.cpp src/shader.h)
target_link_libraries(gl ${SDL2_LIBRARIES} glad ${CMAKE_DL_LIBS})

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
