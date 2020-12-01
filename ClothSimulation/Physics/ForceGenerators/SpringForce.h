#pragma once

#include "../../stdafx.h"
#include "../Particle.h"
#include "IForceGenerator.h"

class SpringForce : public IForceGenerator
{
public:
	SpringForce(int width, int height, float restLength, float stiffness, float damping);
	virtual void ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep) override;
	float Stiffness;
	float Damping;
private:
	float resetLength;
	int width;
	int height;
};