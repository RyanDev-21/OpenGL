cmake_minimum_required(VERSION 3.23)
project(LittleTriangle)

cmake_policy(SET CMP0072 NEW)


find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/triangle"
    DESTINATION "${CMAKE_BINARY_DIR}/shaders"
)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/rectangle"
    DESTINATION "${CMAKE_BINARY_DIR}/shaders"
)

add_executable(LittleTriangle
    src/main.c
    dependencies/glad/src/glad.c
    src/shader/shader.c
    dependencies/imageLib/stb_image.c
)

target_sources(LittleTriangle
    PRIVATE
    src/main.c
    src/shader/shader.c
)

target_include_directories(
    LittleTriangle
    PRIVATE
    dependencies/glad/include
    dependencies/imageLib/
)

target_link_libraries(LittleTriangle glfw m)


