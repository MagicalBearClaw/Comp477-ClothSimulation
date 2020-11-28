#pragma once
#include "../../stdafx.h"
#include "../Particle.h"
#include "IIntergrator.h"

class SemiImplicitEulerIntergrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Particle* body, float timeStep) override;
};
