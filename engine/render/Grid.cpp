#include "Grid.h"
#include <glad/glad.h>
#include <cmath>

Grid::Grid(int size, float spacing) : size(size), spacing(spacing) {
    // Gerar vértices da grade
    for (int z = 0; z < size; ++z) {
        for (int x = 0; x < size; ++x) {
            vertices.push_back(glm::vec3(x * spacing, 0.0f, z * spacing));
        }
    }

    // Gerar índices para triângulos da malha
    for (int z = 0; z < size - 1; ++z) {
        for (int x = 0; x < size - 1; ++x) {
            int i = z * size + x;
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + size);

            indices.push_back(i + 1);
            indices.push_back(i + size + 1);
            indices.push_back(i + size);
        }
    }

    // Buffers OpenGL
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Grid::aplicarGravidade(glm::vec3 posMassa, float massa) {
    for (auto& v : vertices) {
        float distancia = glm::distance(glm::vec2(v.x, v.z), glm::vec2(posMassa.x, posMassa.z));
        float deformacao = -(massa / (distancia + 1.0f)); // evitar divisão por zero
        v.y = deformacao * 0.1f; // escala visual
    }
    atualizarBuffer();
}

void Grid::atualizarBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
}

void Grid::desenhar() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
