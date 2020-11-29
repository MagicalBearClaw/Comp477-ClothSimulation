#pragma once
#include "../../stdafx.h"
#include "../Particle.h"
#include "IIntergrator.h"

class VerletIntegrator : public IIntegrator
{
public:
	VerletIntegrator(float drag);
	// Inherited via IIntegrator
	virtual void Intergrate(Particle* particle, float timeStep) override;
	float Drag;
};
