#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IIntergrator.h"

class SemiImplicitEulerIntergrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Body* body, glm::vec3, float timeStep) override;
private:
};
