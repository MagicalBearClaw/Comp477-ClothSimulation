#pragma once
#include "stdafx.h"
#include "Body.h"


class IIntegrator
{
public:
	virtual void Intergrate(Body* body, DirectX::XMFLOAT3 acceleration, float timeStep) = 0;

};