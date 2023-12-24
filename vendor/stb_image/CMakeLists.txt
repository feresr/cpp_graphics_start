project(stb_image)

## file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)