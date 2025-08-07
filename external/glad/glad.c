// GLAD generated OpenGL loader source file
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
add_library(glad STATIC engine/glad/glad.c)
target_link_libraries(kairos PRIVATE glad glfw)
