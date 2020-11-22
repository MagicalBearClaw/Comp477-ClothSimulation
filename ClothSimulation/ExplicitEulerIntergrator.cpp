#include "stdafx.h"
#include "ExplicitEulerIntergrator.h"

void ExplicitEulerIntergrator::Intergrate(Body* body, DirectX::XMFLOAT3 acceleration, float timeStep)
{
	DirectX::XMVECTOR currentVelocity = DirectX::XMLoadFloat3(&body->GetVelocity());
	DirectX::XMVECTOR scaledVelocity = DirectX::XMVectorScale(currentVelocity, timeStep);
	DirectX::XMFLOAT3 resultingPosition(0, 0, 0);
	DirectX::XMStoreFloat3(&resultingPosition, DirectX::XMVectorAdd(scaledVelocity, DirectX::XMLoadFloat3(&body->GetCurrentPosition())));
	body->SetPosition(resultingPosition);

	DirectX::XMVECTOR scaledAcceleration = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&acceleration), timeStep);
	DirectX::XMFLOAT3 resultingVelocity(0, 0, 0);
	DirectX::XMStoreFloat3(&resultingVelocity, DirectX::XMVectorAdd(scaledAcceleration, currentVelocity));
	body->SetVelocity(resultingVelocity);
}
