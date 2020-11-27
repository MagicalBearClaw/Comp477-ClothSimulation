#pragma once

#include "../stdafx.h"
#include <stb_image.h>
#include "Particle.h"
#include "Constraint.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"

class Cloth 
{
public:
    Cloth(int width, int height, const std::string& textureFileName);
    ~Cloth();
    void Update(float deltaTime);
    void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
    void AddParticlPositionConstraint(unsigned int id);
    void AddCollisionHandler(std::function<void(Particle* particle)> handler);
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
    void LoadTexture(const std::string& textureFileName);
    void Initialize();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateParticles();
    void CreateConstraints();
    glm::vec3 CalculateWindForce();
    void Reset();

private:
    GLuint textureId;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    int height;
    int width; 
    int vertexCount;
    float elapsedTime;
    
    std::string textureFileName;
    std::vector<Particle*> particles;  
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Constraint*> constraints;
    std::vector<std::function<void(Particle* particle)>> collisionHandlers;
};
