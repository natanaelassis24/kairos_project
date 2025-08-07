#pragma once

#include <GLFW/glfw3.h>

class ThreeBodies {
public:
    bool init();
    void run();
    void cleanup();

private:
    GLFWwindow* window = nullptr;
    int width = 800;
    int height = 600;

    void processInput();
    void render();
};
