#pragma once
#include "../../stdafx.h"
#include "../Particle.h"
#include "IIntergrator.h"

class RK4Integrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Particle* body, float timeStep) override;
private:
	struct DerivativeState
	{
		glm::vec3 PositionDelta;
		glm::vec3 VelocityDelta;
	};
private:
	DerivativeState Evaluate(Particle* particle, float timeStep, const DerivativeState& previousDerivativeState);
	DerivativeState Evaluate(Particle* particle);
};
