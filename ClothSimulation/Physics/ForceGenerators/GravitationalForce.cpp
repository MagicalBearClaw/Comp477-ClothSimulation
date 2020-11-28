#include "GravitationalForce.h"

GravitationalForce::GravitationalForce(glm::vec3 gravity) : Gravity(gravity)
{
}

void GravitationalForce::ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep)
{
	particle->Force += particle->Mass * Gravity;
}
