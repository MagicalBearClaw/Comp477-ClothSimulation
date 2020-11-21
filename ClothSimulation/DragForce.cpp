#include "stdafx.h"
#include "DragForce.h"

DragForce::DragForce(float drag) : _drag(drag)
{
}

void DragForce::ApplyForce(Body* body)
{
	DirectX::XMVECTOR previousAppliedForce = DirectX::XMLoadFloat3(&body->GetAppliedForces());
	DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(&body->GetVelocity());
	DirectX::XMVECTOR forceVectore = DirectX::XMVectorScale(velocity, -_drag);

	forceVectore = DirectX::XMVectorAdd(forceVectore, previousAppliedForce);

	DirectX::XMFLOAT3 resultingForce;
	DirectX::XMStoreFloat3(&resultingForce, forceVectore);
	body->SetAppliedForces(resultingForce);
}

float DragForce::GetDrag()
{
    return _drag;
}
