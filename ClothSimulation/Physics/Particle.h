#pragma once
#include "../stdafx.h"

class Particle 
{
public:
    Particle(int vertexId, glm::vec3 position, float mass);
    glm::vec3 Position;
    glm::vec3 PreviousPosition;
    glm::vec3 Force;
    glm::vec3 Acceleration;
    glm::vec3 Velocity;
    float Mass;
    bool IsPositionConstrained;
    int VertexId;
    std::vector<int> GetAjcentParticles(int row, int column);
    std::vector<int> GetDiagonalParticles(int row, int column);
};