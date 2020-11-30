#pragma once

#include "../../stdafx.h"
#include "../Particle.h"
#include "IForceGenerator.h"

class SpringForce : public IForceGenerator
{
public:
	SpringForce(int width, int height, float segmentLengthm, float stiffness);
	virtual void ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep) override;
	float Stiffness;
private:
	float resetLength;
	int width;
	int height;
};