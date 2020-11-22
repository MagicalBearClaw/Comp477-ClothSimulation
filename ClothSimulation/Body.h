#pragma once
#include "stdafx.h"

class Body
{
public:
	enum class BodyType
	{
		Passive,
		Active
	};

	Body(float mass, BodyType bodyType);
	float GetMass();
	BodyType GetType();
	DirectX::XMFLOAT3 GetCurrentPosition();
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetPreviousPosition(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3 GetPreviousPosition();
	DirectX::XMFLOAT3 GetVelocity();
	void SetVelocity(DirectX::XMFLOAT3 velocity);
	void ResetAppliedForces();
	DirectX::XMFLOAT3 GetAppliedForces();
	void SetAppliedForces(DirectX::XMFLOAT3 force);
protected:
	float _mass;
	BodyType _type;
	DirectX::XMFLOAT3 _currentPoistion;
	DirectX::XMFLOAT3 _previousPoistion;
	DirectX::XMFLOAT3 _velocity;
	DirectX::XMFLOAT3 _appliedForces;
};