#include "../stdafx.h"
#include "Sphere.h"

Sphere::Sphere(int resolution, float radius, glm::vec3 position)
              : Radius(radius), resolution(resolution), Position(position)
{
    Initialize();
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Sphere::Draw(Shader& shader, Camera& camera, float radius, glm::vec3 position)
{
    shader.Use();
    
    // Color input
    GLint object_color_loc = glGetUniformLocation(shader.ID,
        "object_color");

    glUniform3f(object_color_loc, 1.0f, 0.5f, 0.2f);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        &vertices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::Update(glm::vec3 newPosition)
{

    for (Vertex& vertex : vertices)
    {
        vertex.Position -= Position;
        vertex.Position += newPosition;
    }
    Position = newPosition;
    //int index = 0;
    //for (int j = 0; j <= resolution; j++)
    //{
    //    for (int i = 0; i <= resolution; i++)
    //    {
    //        Vertex& vertex = vertices[index];
    //        vertex.Position.x = (float)i / resolution;
    //        vertex.Position.y = 0.0f;
    //        vertex.Position.z = (float)j / resolution;

    //        float theta = vertex.Position.x * 2 * glm::pi<float>();
    //        float phi = vertex.Position.z * glm::pi<float>();

    //        vertex.Normal.x = vertex.Position.x = newPosition.x + Radius * glm::cos(theta) * glm::sin(phi);
    //        vertex.Normal.y = vertex.Position.y = newPosition.y + Radius * glm::sin(theta) * glm::sin(phi);
    //        vertex.Normal.z = vertex.Position.z = newPosition.z + Radius * glm::cos(phi);
    //        
    //        index++;
    //    }
    //}
}

void Sphere::Initialize()
{
    Create();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


    glBindVertexArray(0);
}

void Sphere::Create()
{
    for (int j = 0; j <= resolution; j++) 
    {
        for (int i = 0; i <= resolution; i++) 
        {
            Vertex vertex;
            vertex.Position.x = (float)i / resolution;
            vertex.Position.y = 0.0f;
            vertex.Position.z = (float)j / resolution;

            float theta = vertex.Position.x * 2 * glm::pi<float>();
            float phi = vertex.Position.z * glm::pi<float>();

            vertex.Normal.x = vertex.Position.x = Position.x + Radius * glm::cos(theta) * glm::sin(phi);
            vertex.Normal.y = vertex.Position.y = Position.y + Radius * glm::sin(theta) * glm::sin(phi);
            vertex.Normal.z = vertex.Position.z = Position.z + Radius * glm::cos(phi);

            vertex.TexCoords = glm::vec2((float)i / resolution, (float)j / resolution);
            vertices.push_back(vertex);
        }
    }

    for (int j = 0; j < resolution; j++) 
    {
        for (int i = 0; i < resolution; i++) 
        {
            indices.push_back(i + j * (resolution + 1));
            indices.push_back(i + (j + 1) * (resolution + 1));
            indices.push_back(i + 1 + (j + 1) * (resolution + 1));
            indices.push_back(i + j * (resolution + 1));
            indices.push_back(i + 1 + (j + 1) * (resolution + 1));
            indices.push_back(i + 1 + j * (resolution + 1));
        }
    }
}