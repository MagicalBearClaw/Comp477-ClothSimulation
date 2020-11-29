#pragma once
#include "../../stdafx.h"
#include "../Particle.h"
#include "IIntergrator.h"

class ExplicitEulerIntegrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Particle* particle, float timeStep) override;
};
