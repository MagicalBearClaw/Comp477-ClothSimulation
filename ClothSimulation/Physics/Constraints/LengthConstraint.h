#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IConstraint.h"

class LengthConstraint : public IConstraint
{
public:
	LengthConstraint(Body* bodyA, Body* bodyB, float length);
	float GetLength();

	// Inherited via IConstraint
	virtual void SatisfyConstraint() override;

private:
	Body* _bodyA;
	Body* _bodyB;

	float _length;
};
