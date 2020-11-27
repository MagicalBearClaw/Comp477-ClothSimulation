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
    std::vector<int> get_s_neighbors(int i, int col_count, int row_count);
    std::vector<int> get_d_neighbors(int i, int col_count, int row_count);
};