cmake_minimum_required(VERSION 3.12.4)

add_library(glad STATIC glad/glad.h glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

# Header-only libs
add_library(json STATIC json.hpp)
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(json PROPERTIES LINKER_LANGUAGE CXX)

add_library(stb STATIC stb_image.h stb_image_write.h)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(stb PROPERTIES LINKER_LANGUAGE C)

add_library(tiny_gltf STATIC tiny_gltf.h)
target_include_directories(tiny_gltf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(tiny_gltf PROPERTIES LINKER_LANGUAGE CXX)

# External targets
set(BUILD_STATIC_LIBS ON CACHE BOOL "Static libs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Static libs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only")
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")

set(SOLOUD_STATIC ON)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_OPENSLES ON)
set(SOLOUD_BACKEND_MINIAUDIO ON)
set(SOLOUD_BACKEND_SDL2 OFF)
set(SOLOUD_BACKEND_NULL OFF)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_WINMM OFF)
set(SOLOUD_BACKEND_SOLOUD_BACKEND_COREAUDIO OFF)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(soloud)
add_subdirectory(entt)