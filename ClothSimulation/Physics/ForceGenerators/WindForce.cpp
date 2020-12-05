#include "WindForce.h"

WindForce::WindForce(float minimumSpeed, float maximumSpeed) : MinimumSpeed(minimumSpeed), MaximumSpeed(maximumSpeed)
{
}

void WindForce::ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep)
{
    float x = glm::cos(0.1f * ElapsedTime);
    float y = glm::sin(0.1f * ElapsedTime) - 0.2f;
    float z = glm::cos(sin(particle->Position[0] * ElapsedTime) - 0.8f);
    float minSpeed = -1 * MinimumSpeed;
    float maxSpeed = -1 * MaximumSpeed;

    float random = ((float)rand()) / (float)0x7fff;
    float speedDifference = maxSpeed - minSpeed;
    float speedDistribution = random * speedDifference;
    float speed = minSpeed + speedDistribution;

    if (IsEnabled) 
    {
        particle->Force += glm::vec3(speed * x, speed * y, speed * z);
    }
}

