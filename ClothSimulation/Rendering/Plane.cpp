#include "../stdafx.h"
#include "Plane.h"


Plane::Plane(float length, float width, int numberOfLengthSegments, int numberOfWidthSegments, std::string textureFileName) :
			length(length), width(width), NumberOfWidthSegments(numberOfWidthSegments), NumberOfLengthSegments(numberOfLengthSegments), textureFileName(textureFileName)
{
    Initialize();
}

void Plane::CreateVertexBuffer()
{
    vertices.resize((NumberOfLengthSegments + 1)* (NumberOfWidthSegments + 1));
    for (int x = 0; x < NumberOfLengthSegments + 1; x++)
    {
        for (int y = 0; y < NumberOfWidthSegments + 1; y++)
        {
            vertices[x + y * (NumberOfLengthSegments + 1)].Position = glm::vec3(x * gridWidth - length / 2, 0, -y * gridHeight + width / 2);
            vertices[x + y * (NumberOfLengthSegments + 1)].TexCoords = glm::vec2((float)x / NumberOfLengthSegments, (float)y / NumberOfWidthSegments);
            vertices[x + y * (NumberOfLengthSegments + 1)].Normal = glm::vec3(0, 1, 0);
        }
    }

    //create the vertex mapping:
    //for plane, there are no dup vertices, so the pseudo vertex id is just the real vertex id
    for (int i = 0; i < NumberOfVertices; i++)
    {
        vertexMappingPseudoToReal.push_back(std::vector<int>{ i });
    }
}

void Plane::CreateIndexBuffer()
{
    indices.resize(NumberOfLengthSegments * NumberOfWidthSegments * 6);
    for (int x = 0; x < NumberOfLengthSegments; x++)
    {
        for (int y = 0; y < NumberOfWidthSegments; y++)
        {
            //specify the indices for the first tri
            indices[(x + y * NumberOfLengthSegments) * 6] = (x + 1) + (y + 1) * (NumberOfLengthSegments + 1);
            indices[(x + y * NumberOfLengthSegments) * 6 + 1] = (x + 1) + y * (NumberOfLengthSegments + 1);
            indices[(x + y * NumberOfLengthSegments) * 6 + 2] = x + y * (NumberOfLengthSegments + 1);

            //store triangle vertex info for future use
            std::vector<int> triangleVertex(3);
            triangleVertex[0] = indices[(x + y * NumberOfLengthSegments) * 6];
            triangleVertex[1] = indices[(x + y * NumberOfLengthSegments) * 6 + 1];
            triangleVertex[2] = indices[(x + y * NumberOfLengthSegments) * 6 + 2];
            triangleVertexInfo.push_back(triangleVertex);

            //specify the indices for the second tri
            indices[(x + y * NumberOfLengthSegments) * 6 + 3] = (x + 1) + (y + 1) * (NumberOfLengthSegments + 1);
            indices[(x + y * NumberOfLengthSegments) * 6 + 4] = x + y * (NumberOfLengthSegments + 1);
            indices[(x + y * NumberOfLengthSegments) * 6 + 5] = x + (y + 1) * (NumberOfLengthSegments + 1);

            //store triangleVertex info for future use
            triangleVertex[0] = indices[(x + y * NumberOfLengthSegments) * 6 + 3];
            triangleVertex[1] = indices[(x + y * NumberOfLengthSegments) * 6 + 4];
            triangleVertex[2] = indices[(x + y * NumberOfLengthSegments) * 6 + 5];
            triangleVertexInfo.push_back(triangleVertex);
        }
    }
}

void Plane::SetVertexPosition(int vertexId, glm::vec3 position)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Position = position;
    }
}

void Plane::SetVertexNormal(int vertexId, glm::vec3 normal)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Normal = normal;
    }
}

glm::vec3 Plane::GetVertexPosition(int vertexId)
{
    return vertices[vertexMappingPseudoToReal[vertexId][0]].Position;
}

void Plane::AddVertexNormal(int vertexId, glm::vec3 normal)
{
    std::vector<int> realVertexIds = vertexMappingPseudoToReal[vertexId];
    for (int realVertexId : realVertexIds) {
        vertices[realVertexId].Normal += normal;
    }
}

glm::vec3 Plane::GetVertexNormal(int vertexId)
{
    return vertices[vertexMappingPseudoToReal[vertexId][0]].Normal;
}

void Plane::RecalculateNormals()
{
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
    int thisTriVertex1Id;
    int thisTriVertex2Id;
    int thisTriVertex3Id;

    //reset all vertex normals first
    for (int i = 0; i < NumberOfVertices; i++)
    {
        SetVertexNormal(i, glm::vec3(0,0,0));
    }

    //iterate through all tris
    for (int i = 0; i < triangleVertexInfo.size(); i++)
    {
        //get the vertex ids of the current triangle
        thisTriVertex1Id = triangleVertexInfo[i][0];
        thisTriVertex2Id = triangleVertexInfo[i][1];
        thisTriVertex3Id = triangleVertexInfo[i][2];

        //get the positions of those vertices
        point1 = GetVertexPosition(thisTriVertex1Id);
        point2 = GetVertexPosition(thisTriVertex2Id);
        point3 = GetVertexPosition(thisTriVertex3Id);

        //update tri normal
        triangleNormals[i] = -glm::cross(point3 - point2, point1 - point2);
        if (triangleNormals[i] != glm::vec3(0, 0, 0))
        {
            triangleNormals[i] = glm::normalize(triangleNormals[i]);
        }

        //update triangle area
        triangleAreas[i] = triangleNormals[i].length();

        //add the tri normal to the vertices that are sharing it
        AddVertexNormal(thisTriVertex1Id, triangleNormals[i]);
        AddVertexNormal(thisTriVertex2Id, triangleNormals[i]);
        AddVertexNormal(thisTriVertex3Id, triangleNormals[i]);
    }
}

void Plane::Initialize()
{
    gridWidth = (float)length / (float)NumberOfLengthSegments;
    gridHeight = (float)width / (float)NumberOfWidthSegments;

    numRealVertices = (NumberOfLengthSegments + 1) * (NumberOfWidthSegments + 1);
    numTris = NumberOfLengthSegments * NumberOfWidthSegments * 2;
    NumberOfVertices = numRealVertices;
    triangleNormals.resize(numTris);
    triangleAreas.resize(numTris);

    CreateVertexBuffer();
    CreateIndexBuffer();

    LoadTexture(textureFileName);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    RecalculateNormals();
}

void Plane::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    glm::mat4 view = camera.GetViewMatrix();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Plane::LoadTexture(const std::string& textureFileName)
{

    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(textureFileName.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << textureFileName << std::endl;
        stbi_image_free(data);
    }
}
