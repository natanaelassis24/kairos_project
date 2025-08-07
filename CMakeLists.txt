cmake_minimum_required(VERSION 3.10)
project(kairos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Toolchain do vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Users/natan/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Arquivos fonte
add_executable(kairos
    engine/main.cpp
    engine/glad/src/glad.c
)

target_include_directories(kairos PRIVATE engine/glad/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(kairos PRIVATE glfw OpenGL::GL)
