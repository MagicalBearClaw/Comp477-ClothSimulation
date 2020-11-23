#include "../../stdafx.h"
#include "Spring.h"

Spring::Spring(float stiffness, float damping, float restLength) : _stiffness(stiffness), _damping(damping), _restLength(restLength)
{
}

void Spring::ApplyForce(Body* body)
{
	Body& first = body[0];
	Body& second = body[1];

	glm::vec3 direction = first.GetCurrentPosition() - second.GetCurrentPosition();

	if (direction == glm::vec3(0,0,0))
	{
		return;
	}

	float currentLength = direction.length();

	direction = glm::normalize(direction);

	float lengthDifference = currentLength - _restLength;

	glm::vec3 forces = (-_stiffness) * lengthDifference * direction;

	forces += (-_damping) * glm::dot((first.GetVelocity() - second.GetVelocity()), direction) * direction;

	first.SetAppliedForces(first.GetAppliedForces() + forces);
	second.SetAppliedForces(second.GetAppliedForces() + (-forces));
}
