#include "../../stdafx.h"
#include "VerletIntegrator.h"

VerletIntergrator::VerletIntergrator(float drag) : _drag(drag)
{
}

void VerletIntergrator::Intergrate(Body* body, glm::vec3 acceleration, float timeStep)
{
	float twoMinusDrag = 2 - _drag;
	float oneMinusDrag = 1 - _drag;
	float timeStempSquared = timeStep * timeStep;
	glm::vec3 scaledAcceleration = acceleration * timeStempSquared;

	glm::vec3 scaledCurrentPosition = body->GetCurrentPosition() * twoMinusDrag;
	glm::vec3 scaledPreviousPosition = body->GetPreviousPosition() * oneMinusDrag;
	glm::vec3 positionDifference = scaledCurrentPosition - scaledPreviousPosition;
	glm::vec3 resultingPosition = positionDifference + scaledAcceleration;
	body->SetPreviousPosition(body->GetCurrentPosition());
	body->SetPosition(resultingPosition);
}

void VerletIntergrator::SetDrag(float drag)
{
	_drag = drag;

	if (_drag < 0)
		_drag = 0;
	
	if (_drag > 1)
		_drag = 1;
}
