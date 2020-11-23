#include "../../stdafx.h"
#include "PositionConstraint.h"

PositionConstraint::PositionConstraint(Body* body, glm::vec3 position) : _body(body), _position(position)
{
}

void PositionConstraint::SatisfyConstraint()
{
	_body->SetPosition(_position);
}

Body* PositionConstraint::GetBody()
{
	return _body;
}

glm::vec3 PositionConstraint::GetPosition()
{
	return _position;
}
