#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IForceGenerator.h"
#include "../Body.h"

class Spring : IForceGenerator
{
public:
	Spring(float stiffness, float damping, float restLength, Body* bodyA, Body* bodyB);
	// Inherited via IForceGenerator
	virtual void ApplyForce(Body* body) override;
private:
	float _stiffness;
	float _damping;
	float _restLength;
	Body* _bodyA;
	Body* _bodyB;
};
