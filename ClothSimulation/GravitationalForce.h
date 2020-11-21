#pragma once
#include "stdafx.h"
#include "Body.h"
#include "IForceGenerator.h"

class GravitationalForce : public IForceGenerator
{
public:
	GravitationalForce(DirectX::XMFLOAT3 acceleration);
	// Inherited via IForceGenerator
	virtual void ApplyForce(Body* body) override;
	DirectX::XMFLOAT3 GetAcceleration();
private:
	DirectX::XMFLOAT3 _acceleration;
};