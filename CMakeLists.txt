cmake_minimum_required(VERSION 3.0.0)

project(cpp_graphics_start VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
add_subdirectory("vendor/glad")
add_subdirectory("vendor/stb_image")
add_subdirectory("vendor/glm")
add_subdirectory("vendor/assimp")
add_executable(cpp_graphics_start camera.cpp main.cpp Mesh.cpp Model.cpp shader.cpp)
target_include_directories(cpp_graphics_start PUBLIC ".")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(cpp_graphics_start assimp glad stb_image glm ${SDL2})
