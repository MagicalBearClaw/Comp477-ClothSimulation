#include "../../stdafx.h"
#include "ExplicitEulerIntergrator.h"

void ExplicitEulerIntergrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Position += particle->Velocity * timeStep;
    particle->Velocity += particle->Acceleration * timeStep;
}
