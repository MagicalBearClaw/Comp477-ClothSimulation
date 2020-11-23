#include "../../stdafx.h"
#include "SemiImplicitEulerIntegrator.h"

void SemiImplicitEulerIntergrator::Intergrate(Body* body, glm::vec3 acceleration, float timeStep)
{
	glm::vec3 currentVelocity = body->GetVelocity();

	glm::vec3 scaledAcceleration = acceleration * timeStep;
	glm::vec3 resultingVelocity = scaledAcceleration + currentVelocity;
	body->SetVelocity(resultingVelocity);

	glm::vec3 scaledVelocity = resultingVelocity * timeStep;
	glm::vec3 resultingPosition = scaledVelocity + body->GetCurrentPosition();
	body->SetPosition(resultingPosition);
}
