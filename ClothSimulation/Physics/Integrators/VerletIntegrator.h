#pragma once
#include "../../stdafx.h"
#include "../Particle.h"
#include "IIntergrator.h"

class VerletIntergrator : public IIntegrator
{
public:
	VerletIntergrator(float drag);
	// Inherited via IIntegrator
	virtual void Intergrate(Particle* particle, float timeStep) override;
	float Drag;
};
