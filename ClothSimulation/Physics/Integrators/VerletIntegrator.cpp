#include "../../stdafx.h"
#include "VerletIntegrator.h"

VerletIntegrator::VerletIntegrator(float drag) : Drag(drag)
{
}

void VerletIntegrator::Intergrate(Particle* particle, float timeStep)
{
    particle->Position = particle->Position + (1.0f - Drag) 
                         * (particle->Position - particle->PreviousPosition) 
                         + particle->Acceleration * timeStep;
}