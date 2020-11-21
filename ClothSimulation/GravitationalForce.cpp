#include "stdafx.h"
#include "GravitationalForce.h"

GravitationalForce::GravitationalForce(DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, -9.80665f, 0)) : _acceleration(acceleration)
{
}

void GravitationalForce::ApplyForce(Body* body)
{
	DirectX::XMVECTOR previousAppliedForce = DirectX::XMLoadFloat3(&body->GetAppliedForces());
	DirectX::XMVECTOR acceleration = DirectX::XMLoadFloat3(&_acceleration);
	DirectX::XMVECTOR forceVectore = DirectX::XMVectorScale(acceleration, body->GetMass());

	forceVectore = DirectX::XMVectorAdd(forceVectore, previousAppliedForce);

	DirectX::XMFLOAT3 resultingForce;
	DirectX::XMStoreFloat3(&resultingForce, forceVectore);
	body->SetAppliedForces(resultingForce);
}

DirectX::XMFLOAT3 GravitationalForce::GetAcceleration()
{
	return _acceleration;
}
