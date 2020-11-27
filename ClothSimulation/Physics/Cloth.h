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
    void Initialize();

public:
    int NumberOfConstraintIterations;
    float Mass;
    float Stiffness;
    float Damping;
    bool IsWindForceEnabled;
    float SegmentLength;

private:
    void CreateVertexBuffer();
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
    
    std::vector<Particle*> points;  
    std::vector<float> vertices;
    std::vector<int> indices;
    std::vector<Constraint*> constraints;
};
