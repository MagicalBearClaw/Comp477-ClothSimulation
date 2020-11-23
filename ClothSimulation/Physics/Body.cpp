#include "../stdafx.h"
#include "Body.h"

Body::Body(float mass, BodyType bodyType) : _mass(mass), _type(bodyType), _appliedForces(glm::vec3(0,0,0)), 
                                            _currentPoistion(glm::vec3(0,0,0)), _previousPoistion(glm::vec3(0, 0, 0)),
                                            _velocity(glm::vec3(0, 0, 0))
{

}

float Body::GetMass()
{
    return _mass;
}

Body::BodyType Body::GetType()
{
    return _type;
}

glm::vec3 Body::GetCurrentPosition()
{
    return _currentPoistion;
}

void Body::SetPosition(glm::vec3 position)
{
    _currentPoistion;
}

void Body::SetPreviousPosition(glm::vec3 position)
{
    _previousPoistion = position;
}

glm::vec3 Body::GetPreviousPosition()
{
    return _previousPoistion;
}

glm::vec3 Body::GetVelocity()
{
    return _velocity;
}

void Body::SetVelocity(glm::vec3 velocity)
{
    _velocity = velocity;
}

void Body::ResetAppliedForces()
{
    _appliedForces = glm::vec3(0, 0, 0);
}

glm::vec3 Body::GetAppliedForces()
{
    return _appliedForces;
}

void Body::SetAppliedForces(glm::vec3 force)
{
    _appliedForces = force;
}
