#include "SpringForce.h"

SpringForce::SpringForce(int width, int height, float restLength, float stiffness) 
                         : Stiffness(stiffness), width(width), height(height), resetLength(restLength)
{
}

void SpringForce::ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep)
{

    std::vector<int> adjacentParticles = particle->GetAjcentParticles(width, height);

    std::vector<int> diagonalParticles = particle->GetDiagonalParticles(width, height);

    int ajacentParticlesSize = adjacentParticles.size();
    for (int j = 0; j < ajacentParticlesSize; j++)
    {
        Particle* ajacentParticle = particles[adjacentParticles[j]];
        glm::vec3 direction = ajacentParticle->Position - particle->Position;
        particle->Force += glm::normalize(direction)
            * (glm::length(direction) - resetLength) * Stiffness;
    }

    int diagonalParticleSize = diagonalParticles.size();
    for (int j = 0; j < diagonalParticleSize; j++)
    {
        Particle* ajacentParticle = particles[diagonalParticles[j]];
        glm::vec3 direction = ajacentParticle->Position - particle->Position;
        particle->Force += glm::normalize(direction)
            * (glm::length(direction) - resetLength * (float)glm::sqrt(2)) * Stiffness;
    }
}
