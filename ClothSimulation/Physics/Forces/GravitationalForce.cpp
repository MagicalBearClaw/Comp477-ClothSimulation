#include "../../stdafx.h"
#include "GravitationalForce.h"

GravitationalForce::GravitationalForce(glm::vec3 acceleration = glm::vec3(0, -9.80665f, 0)) : _acceleration(acceleration)
{
}

void GravitationalForce::ApplyForce(Body* body)
{
	glm::vec3 previousAppliedForce = body->GetAppliedForces();
	glm::vec3 forceVector = _acceleration * body->GetMass();
	glm::vec3 resultingForce = forceVector + previousAppliedForce;
	body->SetAppliedForces(resultingForce);
}

glm::vec3 GravitationalForce::GetAcceleration()
{
	return _acceleration;
}
