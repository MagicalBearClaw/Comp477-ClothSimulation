#pragma once

#include "../../stdafx.h"
#include "../Particle.h"
#include "IForceGenerator.h"

class WindForce : public IForceGenerator
{
public:
	WindForce(float minimumSpeed, float maximumSpeed);
	virtual void ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep) override;
	float MinimumSpeed;
	float MaximumSpeed;
};