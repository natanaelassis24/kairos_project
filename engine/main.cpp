#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <array>
#include <cmath>

// Vertex shader (GLSL)
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;      // posição do vértice
layout(location = 1) in vec3 aNormal;   // normal para iluminação

out vec3 FragPos;       // posição no mundo para o fragment shader
out vec3 Normal;        // normal para o fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

// Fragment shader (GLSL)
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

// Função para compilar shaders
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erro ao compilar shader: " << infoLog << std::endl;
    }
    return shader;
}

// Função para criar o programa de shaders
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar shader program: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Gera uma esfera simples em latitudes e longitudes
void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int sectorCount = 36, unsigned int stackCount = 18) {
    float PI = 3.14159265359f;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * PI / stackCount; // do pi/2 a -pi/2
        float xy = cosf(stackAngle);
        float z = sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * PI / sectorCount; // de 0 a 2pi

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            // posição
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normal (igual à posição para esfera unitária)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (unsigned int i = 0; i < stackCount; ++i) {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Kairos - Esfera OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Criar shaders
    GLuint shaderProgram = createShaderProgram();

    // Criar malha da esfera
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    generateSphere(vertices, indices);

    // Criar buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Variáveis de transformação
    float angle = 0.0f;

    // Função auxiliar para criar matriz identidade 4x4
    auto identity = []() -> std::array<float, 16> {
        return std::array<float, 16>{
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    // Função para criar matriz perspectiva simples (fov 45°, aspect ratio, near, far)
    auto perspective = [](float fov, float aspect, float near, float far) -> std::vector<float> {
        float tanHalfFov = tanf(fov / 2);
        std::vector<float> mat(16, 0);
        mat[0] = 1 / (aspect * tanHalfFov);
        mat[5] = 1 / tanHalfFov;
        mat[10] = -(far + near) / (far - near);
        mat[11] = -1;
        mat[14] = -(2 * far * near) / (far - near);
        return mat;
    };

    // Função para criar matriz lookAt (camera)
    auto lookAt = [](float eyeX, float eyeY, float eyeZ,
                     float centerX, float centerY, float centerZ,
                     float upX, float upY, float upZ) -> std::vector<float> {
        // vetor direção
        float fx = centerX - eyeX;
        float fy = centerY - eyeY;
        float fz = centerZ - eyeZ;
        float rlf = 1.0f / sqrtf(fx*fx + fy*fy + fz*fz);
        fx *= rlf; fy *= rlf; fz *= rlf;

        // vetor up normalizado
        float rlu = 1.0f / sqrtf(upX*upX + upY*upY + upZ*upZ);
        float ux = upX * rlu;
        float uy = upY * rlu;
        float uz = upZ * rlu;

        // s = f x up
        float sx = fy * uz - fz * uy;
        float sy = fz * ux - fx * uz;
        float sz = fx * uy - fy * ux;

        // u = s x f
        float ux2 = sy * fz - sz * fy;
        float uy2 = sz * fx - sx * fz;
        float uz2 = sx * fy - sy * fx;

        std::vector<float> mat(16, 0);
        mat[0] = sx; mat[1] = ux2; mat[2] = -fx; mat[3] = 0;
        mat[4] = sy; mat[5] = uy2; mat[6] = -fy; mat[7] = 0;
        mat[8] = sz; mat[9] = uz2; mat[10] = -fz; mat[11] = 0;
        mat[12] = 0; mat[13] = 0; mat[14] = 0; mat[15] = 1;

        // tradução
        mat[12] = - (sx*eyeX + sy*eyeY + sz*eyeZ);
        mat[13] = - (ux2*eyeX + uy2*eyeY + uz2*eyeZ);
        mat[14] =   (fx*eyeX + fy*eyeY + fz*eyeZ);

        return mat;
    };

    // Pega os locais dos uniforms
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    // Configura a cor do objeto e da luz
    glUseProgram(shaderProgram);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(objectColorLoc, 0.7f, 0.3f, 0.3f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Rotaciona a esfera
        angle += 0.01f;

        // Cria matriz model - só rotacionar em Y
        float model[16] = {
            cosf(angle), 0, sinf(angle), 0,
            0, 1, 0, 0,
            -sinf(angle), 0, cosf(angle), 0,
            0, 0, 0, 1
        };

        // Cria matriz view (camera)
        std::vector<float> view = lookAt(0,0,3, 0,0,0, 0,1,0);

        // Cria matriz projeção
        float fov = 45.0f * 3.1415926f / 180.0f;
        float aspect = 800.0f / 600.0f;
        std::vector<float> projection = perspective(fov, aspect, 0.1f, 100.0f);

        // Passa as matrizes para os shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data());

        // Passa posições para iluminação
        glUniform3f(lightPosLoc, 1.0f, 1.0f, 2.0f);
        glUniform3f(viewPosLoc, 0.0f, 0.0f, 3.0f);

        // Renderiza a esfera
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
