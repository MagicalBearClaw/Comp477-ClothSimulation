#include "../../stdafx.h"
#include "RK4Integrator.h"

void RK4Integrator::Intergrate(Particle* particle, float timeStep)
{
	particle->Velocity += particle->Acceleration * timeStep;
	glm::vec3 k1 = Evaluate(particle->Position, particle->Velocity, 0);
	glm::vec3 k2 = Evaluate(particle->Position + particle->Velocity, timeStep / 2 * k1, timeStep / 2);
	glm::vec3 k3 = Evaluate(particle->Position + particle->Velocity, timeStep / 2 * k2, timeStep / 2);
	glm::vec3 k4 = Evaluate(particle->Position + particle->Velocity, timeStep * k3, timeStep);

	particle->Position += (timeStep / 6) * (k1 + glm::vec3(2) * k2 + glm::vec3(2) * k3 + k4);
}

glm::vec3 RK4Integrator::Evaluate(glm::vec3 position, glm::vec3 velocity, float timeStep)
{
	position += velocity * timeStep;
	return position;
}
