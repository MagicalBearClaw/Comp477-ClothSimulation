#include "SpringForce.h"

SpringForce::SpringForce(int width, int height, float restLength, float stiffness, float damping)
                         : Stiffness(stiffness), Damping(damping), width(width), height(height), resetLength(restLength)
{
}

void SpringForce::ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep)
{

    std::vector<int> adjacentParticles = particle->GetAjcentParticles(width, height);
    std::vector<int> adjacent2Particles = particle->GetAjcent2Particles(width, height);
    std::vector<int> diagonalParticles = particle->GetDiagonalParticles(width, height);

    int ajacentParticlesSize = adjacentParticles.size();
    for (int j = 0; j < ajacentParticlesSize; j++)
    {
        Particle* ajacentParticle = particles[adjacentParticles[j]];
        glm::vec3 direction = particle->Position - ajacentParticle->Position;

        if (direction == glm::vec3(0, 0, 0))
            continue;

        float currentLength = glm::length(direction);
        direction = glm::normalize(direction);

        glm::vec3 force(0, 0, 0);

        force += direction  * (currentLength - resetLength) * -Stiffness;
        force += -Damping * glm::dot(particle->Velocity - ajacentParticle->Velocity, direction) * direction;
        particle->Force += force;
        ajacentParticle->Force -= force;
    }

    int adjacent2ParticlesSize = adjacent2Particles.size();
    for (int j = 0; j < adjacent2ParticlesSize; j++)
    {
        Particle* ajacentParticle = particles[adjacent2Particles[j]];
        glm::vec3 direction = particle->Position - ajacentParticle->Position;
     
        if (direction == glm::vec3(0, 0, 0))
            continue;

        float currentLength = glm::length(direction);
        direction = glm::normalize(direction);
        
        glm::vec3 force(0, 0, 0);

        force += direction * (currentLength - resetLength * 2) * -Stiffness;
        force += -Damping * glm::dot(particle->Velocity - ajacentParticle->Velocity, direction) * direction;
        particle->Force += force;
        ajacentParticle->Force -= force;
    }

    int diagonalParticleSize = diagonalParticles.size();
    for (int j = 0; j < diagonalParticleSize; j++)
    {
        Particle* ajacentParticle = particles[diagonalParticles[j]];
        glm::vec3 direction = particle->Position - ajacentParticle->Position;

        if (direction == glm::vec3(0, 0, 0))
            continue;

        float currentLength = glm::length(direction);
        direction = glm::normalize(direction);

        glm::vec3 force(0, 0, 0);

        force += direction * (currentLength - resetLength * (float)glm::sqrt(2)) * -Stiffness;
        force += -Damping * glm::dot(particle->Velocity - ajacentParticle->Velocity, direction) * direction;
        particle->Force += force;
        ajacentParticle->Force -= force;
    }
}
