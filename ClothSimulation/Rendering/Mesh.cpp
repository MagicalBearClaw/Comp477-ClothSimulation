#include "../stdafx.h"
#include "Mesh.h"

Mesh::Mesh(std::string textureFileName) : textureFileName(textureFileName)
{

}

void Mesh::SetVertexPosition(int vertexId, glm::vec3 position)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Position = position;
    }

}

void Mesh::SetVertexNormal(int vertexId, glm::vec3 normal)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Normal = normal;
    }
}

glm::vec3 Mesh::GetVertexPosition(int vertexId)
{
    return vertices[vertexMappingPseudoToReal[vertexId][0]].Position;
}

void Mesh::AddVertexNormal(int vertexId, glm::vec3 normal)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Normal += normal;
    }
}

glm::vec3 Mesh::GetVertexNormal(int vertexId)
{
    return glm::vec3();
}

void Mesh::RecalculateNormals()
{
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
    int thisTriVertex1Id;
    int thisTriVertex2Id;
    int thisTriVertex3Id;
    //reset all vertex normals first

    for (int i = 0; i < numberOfVerticies; i++)
    {
        SetVertexNormal(i, glm::vec3());
    }

    ////iterate through all tris
    //for (int i = 0; i < triangleVertexInfo.size(); i++)
    //{
    //    //get the vertex ids of the current triangle
    //    //thisTriVertex1Id = std::get<0>(triangleVertexInfo[i]);
    //    //thisTriVertex2Id = std::get<1>(triangleVertexInfo[i]);
    //    //thisTriVertex3Id = std::get<2>(triangleVertexInfo[i]);

    //    //get the positions of those vertices
    //    point1 = GetVertexPosition(thisTriVertex1Id);
    //    point2 = GetVertexPosition(thisTriVertex2Id);
    //    point3 = GetVertexPosition(thisTriVertex3Id);
    //    //point1 = this.vertices[thisTriVertex1Id].Position;
    //    //point2 = this.vertices[thisTriVertex2Id].Position;
    //    //point3 = this.vertices[thisTriVertex3Id].Position;

    //    //update tri normal
    //    triangleNormals[i] = -glm::cross(point3 - point2, point1 - point2);
    //    if (triangleNormals[i] != glm::vec3())
    //    {
    //        triangleNormals[i] = glm::normalize(triangleNormals[i]);
    //    }

    //    //update triangle area
    //    triangleAreas[i] = triangleNormals[i].length();

    //    //add the tri normal to the vertices that are sharing it
    //    AddVertexNormal(thisTriVertex1Id, triangleNormals[i]);
    //    AddVertexNormal(thisTriVertex2Id, triangleNormals[i]);
    //    AddVertexNormal(thisTriVertex3Id, triangleNormals[i]);
    //}
}

void Mesh::Update()
{
}

float Mesh::RecalculateVolume()
{
    float volume = 0;
    //glm::vec3 point1;
    //glm::vec3 point2;
    //glm::vec3 point3;
    //for (int i = 0; i < numberOfFaces; i++)
    //{
    //    point1 = GetVertexPosition(std::get<0>(triangleVertexInfo[i]));
    //    point2 = GetVertexPosition(std::get<1>(triangleVertexInfo[i]));
    //    point3 = GetVertexPosition(std::get<2>(triangleVertexInfo[i]));
    //    volume += triangleAreas[i] * triangleNormals[i].x * (point1.x + point2.x + point3.x);
    //}
    return volume / 3;
}

void Mesh::Initialize()
{
}

void Mesh::Draw(float deltaTime)
{
}
