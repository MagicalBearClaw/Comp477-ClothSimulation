#include "../../stdafx.h"
#include "ExplicitEulerIntegrator.h"

void ExplicitEulerIntegrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Position += particle->Velocity * timeStep;
    particle->Velocity += particle->Acceleration * timeStep;
}
