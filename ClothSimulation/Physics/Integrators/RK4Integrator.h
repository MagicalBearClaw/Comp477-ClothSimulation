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
	glm::vec3 Evaluate(glm::vec3 position, glm::vec3 velocity, float timeStep);

};
