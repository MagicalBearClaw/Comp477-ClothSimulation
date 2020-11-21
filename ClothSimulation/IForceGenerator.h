#pragma once
class IForceGenerator
{
public:
	virtual void ApplyForce(class Body* body) = 0;
};