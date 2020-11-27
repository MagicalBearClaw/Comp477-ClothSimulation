#include "../stdafx.h"
#include "Particle.h"


Particle::Particle(glm::vec3 position, unsigned int vertexId) {
    CurrentPosition = position;
    PreviousPosition = CurrentPosition;
    IsPositionConstrained = false;
    VertexId = vertexId;
}
