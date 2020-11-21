#pragma once
#include "stdafx.h"
#include "Body.h"
#include "IIntergrator.h"

class ForwardEulerIntegrator : public IIntegrator
{
public:
	// Inherited via IIntegrator
	virtual void Intergrate(Body* body, DirectX::XMFLOAT3 acceleration, float timeStep) override;
private:
};
