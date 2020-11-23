#include "../../stdafx.h"
#include "DragForce.h"

DragForce::DragForce(float drag) : _drag(drag)
{
}

void DragForce::ApplyForce(Body* body)
{
	glm::vec3 force = (-_drag) * body->GetVelocity();
	body->SetAppliedForces(body->GetAppliedForces() + force);
}

float DragForce::GetDrag()
{
    return _drag;
}
