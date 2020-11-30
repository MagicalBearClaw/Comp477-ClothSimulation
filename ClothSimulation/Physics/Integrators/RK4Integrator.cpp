#include "../../stdafx.h"
#include "RK4Integrator.h"

void RK4Integrator::Intergrate(Particle* particle, float timeStep)
{
	DerivativeState k1 = Evaluate(particle);
	DerivativeState k2 = Evaluate(particle, timeStep / 2, k1);
	DerivativeState k3 = Evaluate(particle, timeStep / 2, k2);
	DerivativeState k4 = Evaluate(particle, timeStep, k3);

	particle->Position += (1.0f / 6.0f * (k1.PositionDelta + 2.0f * (k2.PositionDelta + k3.PositionDelta) + k4.PositionDelta)) * timeStep;
	particle->Velocity += (1.0f / 6.0f * (k1.VelocityDelta + 2.0f * (k2.VelocityDelta + k3.VelocityDelta) + k4.VelocityDelta)) * timeStep;
}

RK4Integrator::DerivativeState RK4Integrator::Evaluate(Particle* particle, float timeStep, const DerivativeState& previousDerivativeState)
{
	return { particle->Velocity + previousDerivativeState.VelocityDelta * timeStep, particle->Acceleration };
}

RK4Integrator::DerivativeState RK4Integrator::Evaluate(Particle* particle)
{
	return { particle->Velocity , particle->Acceleration };
}