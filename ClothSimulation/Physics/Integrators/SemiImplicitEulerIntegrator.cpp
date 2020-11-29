#include "../../stdafx.h"
#include "SemiImplicitEulerIntegrator.h"

void SemiImplicitEulerIntegrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Velocity += particle->Acceleration * timeStep;
    particle->Position += particle->Velocity * timeStep;
}
