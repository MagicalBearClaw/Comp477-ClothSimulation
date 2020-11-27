#pragma once
#include "../stdafx.h"
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
class Sphere
{
public:
    Sphere(const std::string textureFileName, float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
    ~Sphere();
    void Draw(Shader& shader, Camera& camera, glm::mat4 model, glm::mat4 projection);
private:
    void LoadTexture(const std::string& textureFileName);
    void Initialize();
    void Create();
private:
    float radius;
    int sectorCount;
    int stackCount;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string textureFileName;
    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};