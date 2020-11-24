#pragma once
#include "../stdafx.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};



class Mesh
{
protected:

    Mesh(std::string textureFileName);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int numberofVerticesWithDuplicates;  //number of vertices in the buffer (might have dups)
    int numberOfFaces;
    int numberOfVerticies;  //number of pseudo vertices (no dups)
    std::vector<glm::vec3> triangleNormals;
    //std::vector<std::array<int,3>> triangleVertexInfo;
    std::vector<std::vector<int>> vertexMappingPseudoToReal;
    float* triangleAreas;
    std::string textureFileName;
public:
     virtual void CreateVertexBuffer() = 0;
     virtual void CreateIndexBuffer() = 0;
     void SetVertexPosition(int vertexId, glm::vec3 position);
     void SetVertexNormal(int vertexId, glm::vec3 normal);
     glm::vec3 GetVertexPosition(int vertexId);

     void AddVertexNormal(int vertexId, glm::vec3 normal);
     glm::vec3 GetVertexNormal(int vertexId);
     void RecalculateNormals();
     void Update();
     float RecalculateVolume();
     void Initialize();
     virtual void Draw(float deltaTime);
};