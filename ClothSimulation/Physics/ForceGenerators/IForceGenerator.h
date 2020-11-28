#pragma once
#include "../../stdafx.h"
#include "../Particle.h"

class IForceGenerator
{
public:
	virtual void ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep) = 0;
	bool IsEnabled = false;
	float ElapsedTime = 0.0f;
};