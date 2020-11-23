#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IIntergrator.h"

class VerletIntergrator : public IIntegrator
{
public:
	VerletIntergrator(float drag);
	// Inherited via IIntegrator
	virtual void Intergrate(Body* body, glm::vec3 acceleration, float timeStep) override;
	void SetDrag(float drag);
private:
	float _drag;
};
