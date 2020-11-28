#include "../../stdafx.h"
#include "SemiImplicitEulerIntegrator.h"

void SemiImplicitEulerIntergrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Velocity += particle->Acceleration * timeStep;
    particle->Position += particle->Velocity * timeStep;
}
