#include "SpringForce.h"

SpringForce::SpringForce(int width, int height, float segmentLength, float stiffness) 
                         : Stiffness(stiffness), width(width), height(height), segmentLength(segmentLength)
{
}

void SpringForce::ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep)
{

    std::vector<int> adjacentParticles = particle->GetAjcentParticles(width, height);

    std::vector<int> diagonalParticles = particle->GetDiagonalParticles(width, height);

    int ajacentParticlesSize = adjacentParticles.size();
    for (int j = 0; j < ajacentParticlesSize; j++)
    {
        Particle* neighbor_point = particles[adjacentParticles[j]];
        glm::vec3 x = neighbor_point->Position - particle->Position;
        particle->Force += glm::normalize(x)
            * (glm::length(x) - segmentLength)
            * Stiffness;
    }

    int diagonalParticleSize = diagonalParticles.size();
    for (int j = 0; j < diagonalParticleSize; j++)
    {
        Particle* neighbor_point = particles[diagonalParticles[j]];
        glm::vec3 x = neighbor_point->Position - particle->Position;
        particle->Force += glm::normalize(x)
            * (glm::length(x) - segmentLength * (float)glm::sqrt(2))
            * Stiffness;
    }
}
