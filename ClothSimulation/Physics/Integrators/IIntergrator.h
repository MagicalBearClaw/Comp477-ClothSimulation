#pragma once
#include "../../stdafx.h"
#include "../Particle.h"


class IIntegrator
{
public:
	virtual void Intergrate(Particle* body, float timeStep) = 0;

};