#pragma once

#include "../stdafx.h"
#include "Particle.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"
#include "Constraint.h"

class Cloth 
{
public:
    Cloth(int width, int height);
    ~Cloth();
    void Update(float deltaTime);
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    void AddParticlPositionConstraint(unsigned int id);
public:
    int NumberOfConstraintIterations;
    float Mass;
    float Stiffness;
    float Damping;
    bool IsWindForceEnabled;
    float SegmentLength;

private:
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
    };
    void Initialize();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateParticles();
    void CreateConstraints();
    glm::vec3 CalculateWindForce();

private:
    GLuint textureId;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    int height;
    int width; 
    int vertexCount;
    float elapsedTime;
    
    std::vector<Particle*> particles;  
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Constraint*> constraints;
};
