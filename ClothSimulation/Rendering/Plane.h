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

class Plane
{
public: 
    Plane(float length, float width, int numberOfWidthSegments, int numberOfLengthSegments, std::string textureFileName);
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void SetVertexPosition(int vertexId, glm::vec3 position);
    void SetVertexNormal(int vertexId, glm::vec3 normal);
    glm::vec3 GetVertexPosition(int vertexId);

    void AddVertexNormal(int vertexId, glm::vec3 normal);
    glm::vec3 GetVertexNormal(int vertexId);
    void RecalculateNormals();
    float RecalculateVolume();
    void Initialize();
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);

private:
    void LoadTexture(const std::string& textureFileName);
private:
    int numRealVertices;  //number of vertices in the buffer (might have dups)
    int numTris;
    int numVertices;  //number of pseudo vertices (no dups)

    float width;
    float length;
    int numberOfWidthSegments;
    int numberOfLengthSegments;
    float gridWidth;
    float gridHeight;
    GLuint textureId;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;


    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<std::vector<int>> vertexMappingPseudoToReal;
    std::vector<std::vector<int>> triangleVertexInfo;
    std::vector<float> triangleAreas;
    std::vector<glm::vec3> triangleNormals;
    std::string textureFileName;
};