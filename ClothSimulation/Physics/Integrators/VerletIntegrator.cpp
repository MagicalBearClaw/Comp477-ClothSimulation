#include "../../stdafx.h"
#include "VerletIntegrator.h"

VerletIntergrator::VerletIntergrator(float drag) : Drag(drag)
{
}

void VerletIntergrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Position = particle->Position + (1.0f - Drag) 
                         * (particle->Position - particle->PreviousPosition) 
                         + particle->Acceleration * timeStep;
}