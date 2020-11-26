#pragma once
#include "../stdafx.h"
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"



class Plane
{
    friend class Cloth;
public: 
    Plane(int width, int height, std::string textureFileName);
    void CreateBuffers();
    void Initialize();
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    int NumberOfVertices;
    int NumberOfIndicies;
    int Width;
    int Height;
    float PlaneStride;
private:
    struct Vertex {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };
    void LoadTexture(const std::string& textureFileName);
private:
    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::string _textureFileName;
};