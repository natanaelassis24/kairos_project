# Projeto KAIROS

Motor gráfico básico com GLFW e GLAD integrados.

## Como compilar

1. Instale o GLFW (https://www.glfw.org/)
2. Baixe e configure o GLAD (https://glad.dav1d.de/), ou use a pasta `external/glad`
3. Configure o CMake para encontrar GLFW e GLAD
4. Execute:

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Como rodar

Execute o executável gerado na pasta `build`.

## Estrutura do projeto

- `engine/` : código fonte principal
- `external/glad/` : GLAD OpenGL loader
- `CMakeLists.txt` : build system
