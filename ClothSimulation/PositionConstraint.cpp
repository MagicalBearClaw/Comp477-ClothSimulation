#include "stdafx.h"
#include "PositionConstraint.h"

PositionConstraint::PositionConstraint(Body* body, DirectX::XMFLOAT3 position) : _body(body), _position(position)
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

DirectX::XMFLOAT3 PositionConstraint::GetPosition()
{
	return _position;
}
