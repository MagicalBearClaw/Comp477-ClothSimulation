#pragma once
#include "../stdafx.h"
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"


class Sphere
{
public:
    Sphere(int resolution, float radius, glm::vec3 position, const std::string textureFileName);
    ~Sphere();
    void Draw(Shader& shader, Camera& camera, float radius, glm::vec3 position);
    void Update(glm::vec3 position);
    float Radius;
    glm::vec3 Position;
private:
    void LoadTexture(const std::string& textureFileName);
    void Initialize();
    void Create();
private:
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    int resolution;
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::string textureFileName;

    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};