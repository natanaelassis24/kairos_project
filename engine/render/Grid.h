#pragma once
#include <vector>
#include <glm/glm.hpp>

class Grid {
public:
    Grid(int size, float spacing);
    void aplicarGravidade(glm::vec3 posicaoMassa, float massa);
    void atualizarBuffer();
    void desenhar();

private:
    int size;
    float spacing;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    unsigned int vao, vbo, ebo;
};
