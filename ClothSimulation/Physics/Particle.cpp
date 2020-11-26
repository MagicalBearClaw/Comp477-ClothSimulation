#include "../stdafx.h"
#include "Particle.h"


Particle::Particle(glm::vec3 position, glm::vec2 texcoords) {
    CurrentPosition = position;
    PreviousPosition = CurrentPosition;
    IsPositionConstrained = false;
    TexCoords = texcoords;
    VertexId = 0;
}
