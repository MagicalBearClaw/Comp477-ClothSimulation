#pragma once

#include "../../stdafx.h"
#include "../Particle.h"
#include "IForceGenerator.h"

class GravitationalForce : public IForceGenerator
{
public:
	GravitationalForce(glm::vec3 gravity);
	virtual void ApplyForce(Particle* particle, std::vector<Particle*>& particles, float timeStep) override;
	glm::vec3 Gravity;
};