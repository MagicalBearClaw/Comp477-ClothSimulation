#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IForceGenerator.h"

class GravitationalForce : public IForceGenerator
{
public:
	GravitationalForce(glm::vec3 acceleration);
	// Inherited via IForceGenerator
	virtual void ApplyForce(Body* body) override;
	glm::vec3 GetAcceleration();
private:
	glm::vec3 _acceleration;
};