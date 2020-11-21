#pragma once
#include "stdafx.h"
#include "Body.h"
#include "IForceGenerator.h"

class DragForce : public IForceGenerator
{
public:
	DragForce(float drag);
	// Inherited via IForceGenerator
	virtual void ApplyForce(Body* body) override;
	float GetDrag();
private:
	float _drag;
};