#pragma once
#include "../stdafx.h"

class Body
{
public:
	Body(float mass);
	float GetMass();
	glm::vec3 GetCurrentPosition();
	void SetPosition(glm::vec3 position);
	void SetPreviousPosition(glm::vec3 position);
	glm::vec3 GetPreviousPosition();
	glm::vec3 GetVelocity();
	void SetVelocity(glm::vec3 velocity);
	void ResetAppliedForces();
	glm::vec3 GetAppliedForces();
	void SetAppliedForces(glm::vec3 force);
protected:
	float _mass;
	glm::vec3 _currentPoistion;
	glm::vec3 _previousPoistion;
	glm::vec3 _velocity;
	glm::vec3 _appliedForces;
};