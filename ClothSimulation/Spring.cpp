#include "stdafx.h"
#include "Spring.h"

Spring::Spring(float stiffness, float damping, float restLength) : _stiffness(stiffness), _damping(damping), _restLength(restLength)
{
}

void Spring::ApplyForce(Body* body)
{
	Body& first = body[0];
	Body& second = body[1];

	float currentLength = 0;
	DirectX::XMFLOAT3 forces(0,0,0);

	DirectX::XMVECTOR position1 = DirectX::XMLoadFloat3(&first.GetCurrentPosition());
	DirectX::XMVECTOR position2 = DirectX::XMLoadFloat3(&second.GetCurrentPosition());

	DirectX::XMVECTOR directionVector = DirectX::XMVectorSubtract(position1, position2);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVector);

	if (isVector3AllZeros(direction))
	{
		return;
	}

	DirectX::XMFLOAT3 directionLength;
	DirectX::XMStoreFloat3(&directionLength, DirectX::XMVector3Length(directionVector));
	currentLength = directionLength.x; // does not matter which element we choose as the length is replicated in all 3 elements

	directionVector = DirectX::XMVector3Normalize(directionVector);

	float lengthDifference = currentLength - _restLength;

	DirectX::XMVECTOR scaledDirection = DirectX::XMVectorScale(directionVector, lengthDifference);
	scaledDirection = DirectX::XMVectorScale(directionVector, -_stiffness);

	DirectX::XMStoreFloat3(&forces, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&forces), scaledDirection));

	DirectX::XMVECTOR veclocity1 = DirectX::XMLoadFloat3(&first.GetVelocity());
	DirectX::XMVECTOR veclocity2 = DirectX::XMLoadFloat3(&second.GetVelocity());
	DirectX::XMVECTOR dirVelDOtProdVector = DirectX::XMVector3Dot(DirectX::XMVectorSubtract(veclocity1, veclocity2), directionVector);

	float dotProdValue = DirectX::XMVectorGetX(dirVelDOtProdVector);

	scaledDirection = DirectX::XMVectorScale(directionVector, dotProdValue);
	scaledDirection = DirectX::XMVectorScale(directionVector, -_damping);

	DirectX::XMVECTOR forceVector = DirectX::XMLoadFloat3(&forces);
	DirectX::XMStoreFloat3(&forces, DirectX::XMVectorAdd(forceVector, scaledDirection));

	DirectX::XMFLOAT3 firstResultingForce(0, 0, 0);
	DirectX::XMStoreFloat3(&firstResultingForce, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&first.GetAppliedForces()), scaledDirection));
	first.SetAppliedForces(firstResultingForce);

	DirectX::XMFLOAT3 secondResultingForce(0, 0, 0);
	DirectX::XMStoreFloat3(&secondResultingForce, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&second.GetAppliedForces()), DirectX::XMVectorNegate(scaledDirection)));
	first.SetAppliedForces(secondResultingForce);

}
