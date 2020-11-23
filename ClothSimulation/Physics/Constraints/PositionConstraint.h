#pragma once
#include "../../stdafx.h"
#include "../Body.h"
#include "IConstraint.h"

class PositionConstraint : public IConstraint
{
public:
	PositionConstraint(Body* body, glm::vec3 position);

	// Inherited via IConstraint
	virtual void SatisfyConstraint() override;
	Body* GetBody();
	glm::vec3 GetPosition();
private:
	Body* _body;
	glm::vec3 _position;
};
