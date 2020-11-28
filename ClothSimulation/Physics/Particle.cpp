#include "../stdafx.h"
#include "Particle.h"

Particle::Particle(int vertexId, glm::vec3 position, float mass)
{
    Position = position;
    PreviousPosition = Position;
    IsPositionConstrained = false;
    VertexId = vertexId;
    Mass = mass;
}
std::vector<int> Particle::GetAjcentParticles(int gridWidth, int gridHeight) 
{
    std::vector<int> particles;

    int row = VertexId / gridWidth;
    int column = VertexId - row * gridWidth;

    int up = (row - 1) * gridWidth + column;
    if (row > 0)
    {
        particles.push_back(up);
    }
    
    int down = (row + 1) * gridWidth + column;
    if (row < gridHeight - 1)
    {
        particles.push_back(down);
    }
    
    int left = row * gridWidth + column - 1;
    if (column > 0)
    {
        particles.push_back(left);
    }

    int right = row * gridWidth + column + 1;
    if (column < gridWidth - 1)
    {
        particles.push_back(right);
    }
    
    return particles;
}

std::vector<int> Particle::GetDiagonalParticles(int gridWidth, int gridHeight)
{
    std::vector<int> particles;

    int row = VertexId / gridWidth;
    int column = VertexId - row * gridWidth;
    
    int up_left = (row - 1) * gridWidth + column - 1;
    if (row > 0 && column > 0)
    {
        particles.push_back(up_left);
    }
    
    int down_right = (row + 1) * gridWidth + column + 1;
    if (row < gridHeight - 1 && column < gridWidth - 1)
    {
        particles.push_back(down_right);
    }
    int up_right = (row - 1) * gridWidth + column + 1;
    if (row > 0 && column < gridWidth - 1)
    {
        particles.push_back(up_right);
    }
    int down_left = (row + 1) * gridWidth + column - 1;
    if (column > 0 && row < gridHeight - 1)
    {
        particles.push_back(down_left);
    }
    return particles;
}
