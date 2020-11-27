#include "../stdafx.h"
#include "Constraint.h"

Constraint::Constraint(int vertexIdA, int vertexIdB, float restLength)
{
    this->VertexIdA = vertexIdA;
    this->VertexIdB = vertexIdB;
    this->restLength = restLength;
}

void Constraint::SatisfyConstraints(Particle* particleA, Particle* particleB)
{
    float rest_distance = restLength;
    float distance = glm::length(particleA->Position - particleB->Position);
    if (distance > rest_distance) {
        float offset = (distance - rest_distance) / distance;
        glm::vec3 correction = 0.5f * offset * (particleA->Position - particleB->Position);
        if (!particleA->IsPositionConstrained)
            particleA->Position -= correction;
        if (!particleB->IsPositionConstrained)
            particleB->Position += correction;
    }

    float tear_distance = restLength * 3.0f;
    if (distance > tear_distance) {
        float offset = (distance - rest_distance) / distance;
        glm::vec3 correction = offset * (particleA->Position - particleB->Position);
        if (particleA->IsPositionConstrained) particleA->IsPositionConstrained = false;
        if (particleB->IsPositionConstrained) particleB->IsPositionConstrained = false;
        //constraints.erase(constraints.begin() + i);
    }
}