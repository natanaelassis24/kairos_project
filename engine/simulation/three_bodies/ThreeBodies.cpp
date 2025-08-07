#include <glad/glad.h>      // INCLUIR PRIMEIRO
#include <GLFW/glfw3.h>
#include <iostream>
#include "ThreeBodies.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool ThreeBodies::init() {
    if (!glfwInit()) {
        std::cerr << "Erro ao iniciar GLFW\n";
        return false;
    }

    // OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, "Simulacao 3 Corpos - KAIROS", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao iniciar GLAD\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    std::cout << "Janela criada com sucesso\n";

    return true;
}

void ThreeBodies::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void ThreeBodies::render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Renderizar 3 corpos (cubos coloridos simples como protótipo)

    float cubePositions[3][3] = {
        {-0.5f, 0.0f, -2.0f},
        {0.5f, 0.0f, -2.0f},
        {0.0f, 0.5f, -2.5f},
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 3; i++) {
        float x = cubePositions[i][0];
        float y = cubePositions[i][1];
        float z = cubePositions[i][2];

        glColor3f(i == 0, i == 1, i == 2);

        // Frente
        glVertex3f(x - 0.1f, y - 0.1f, z + 0.1f);
        glVertex3f(x + 0.1f, y - 0.1f, z + 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z + 0.1f);
        glVertex3f(x - 0.1f, y + 0.1f, z + 0.1f);
        // Traseira
        glVertex3f(x - 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z - 0.1f);
        glVertex3f(x - 0.1f, y + 0.1f, z - 0.1f);
        // Esquerda
        glVertex3f(x - 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x - 0.1f, y - 0.1f, z + 0.1f);
        glVertex3f(x - 0.1f, y + 0.1f, z + 0.1f);
        glVertex3f(x - 0.1f, y + 0.1f, z - 0.1f);
        // Direita
        glVertex3f(x + 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y - 0.1f, z + 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z + 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z - 0.1f);
        // Topo
        glVertex3f(x - 0.1f, y + 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y + 0.1f, z + 0.1f);
        glVertex3f(x - 0.1f, y + 0.1f, z + 0.1f);
        // Base
        glVertex3f(x - 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y - 0.1f, z - 0.1f);
        glVertex3f(x + 0.1f, y - 0.1f, z + 0.1f);
        glVertex3f(x - 0.1f, y - 0.1f, z + 0.1f);
    }
    glEnd();
}

void ThreeBodies::run() {
    if (!window) return;

    while (!glfwWindowShouldClose(window)) {
        processInput();

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void ThreeBodies::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}
