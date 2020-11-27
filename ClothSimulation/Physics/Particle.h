#pragma once
#include "../stdafx.h"

struct Particle 
{
public:
    Particle(glm::vec3 position, unsigned int vertexId);
    glm::vec3 CurrentPosition;
    glm::vec3 PreviousPosition;
    glm::vec3 Force;
    bool IsPositionConstrained;
    int VertexId;
    glm::vec3 Acceleration;
};
