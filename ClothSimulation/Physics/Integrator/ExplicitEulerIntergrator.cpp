#include "../../stdafx.h"
#include "ExplicitEulerIntergrator.h"

void ExplicitEulerIntergrator::Intergrate(Body* body, glm::vec3 acceleration, float timeStep)
{
	glm::vec3 currentVelocity = body->GetVelocity();
	glm::vec3 scaledVelocity = currentVelocity * timeStep;
	glm::vec3 resultingPosition = scaledVelocity + body->GetCurrentPosition();
	body->SetPosition(resultingPosition);

	glm::vec3 scaledAcceleration = acceleration * timeStep;
	glm::vec3 resultingVelocity = scaledVelocity + currentVelocity;
	body->SetVelocity(resultingVelocity);
}
