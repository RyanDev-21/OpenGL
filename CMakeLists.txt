cmake_minimum_required(VERSION 3.23)
project(LittleTriangle)

cmake_policy(SET CMP0072 NEW)


find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)


add_executable(LittleTriangle
    src/main.cpp
    dependencies/glad/src/glad.c
)

target_sources(LittleTriangle
    PRIVATE
    src/main.cpp
)

target_include_directories(
    LittleTriangle
    PRIVATE
    dependencies/glad/include
)

target_link_libraries(LittleTriangle glfw)


