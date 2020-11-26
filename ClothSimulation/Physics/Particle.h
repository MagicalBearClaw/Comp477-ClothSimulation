#pragma once
#include "../stdafx.h"

struct Particle 
{
public:
    Particle(glm::vec3 position, glm::vec2 texcoords);
    glm::vec3 CurrentPosition;
    glm::vec3 PreviousPosition;
    glm::vec2 TexCoords;
    glm::vec3 Force;
    bool IsPositionConstrained;
    int VertexId;
    glm::vec3 Acceleration;
};
