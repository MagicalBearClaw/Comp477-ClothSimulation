#pragma once
#include "stdafx.h"
#include "Body.h"
#include "IConstraint.h"

class PositionConstraint : public IConstraint
{
public:
	PositionConstraint(Body* body, DirectX::XMFLOAT3 position);

	// Inherited via IConstraint
	virtual void SatisfyConstraint() override;
	Body* GetBody();
	DirectX::XMFLOAT3 GetPosition();
private:
	Body* _body;
	DirectX::XMFLOAT3 _position;
};
