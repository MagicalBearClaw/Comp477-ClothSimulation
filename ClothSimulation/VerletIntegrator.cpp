#include "stdafx.h"
#include "VerletIntegrator.h"

VerletIntergrator::VerletIntergrator(float drag) : _drag(drag)
{
}

void VerletIntergrator::Intergrate(Body* body, DirectX::XMFLOAT3 acceleration, float timeStep)
{
	DirectX::XMFLOAT3 resultingPosition(0,0,0);
	float twoMinusDrag = 2 - _drag;
	float oneMinusDrag = 1 - _drag;
	DirectX::XMVECTOR scaledCurrentPosition = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&body->GetCurrentPosition()), twoMinusDrag);
	DirectX::XMVECTOR scaledPreviousPosition = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&body->GetPreviousPosition()), oneMinusDrag);
	DirectX::XMVECTOR positionDifference = DirectX::XMVectorSubtract(scaledCurrentPosition, scaledPreviousPosition);
	float timeStempSquared = timeStep * timeStep;
	DirectX::XMVECTOR scaledAcceleration = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&acceleration), timeStempSquared);
	DirectX::XMStoreFloat3(&resultingPosition, DirectX::XMVectorAdd(positionDifference, scaledAcceleration));
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
