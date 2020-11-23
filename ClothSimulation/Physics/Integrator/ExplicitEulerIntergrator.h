#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IIntergrator.h"

class ExplicitEulerIntergrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Body* body, glm::vec3 acceleration, float timeStep) override;
private:
};
