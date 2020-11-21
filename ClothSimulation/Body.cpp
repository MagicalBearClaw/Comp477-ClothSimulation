#include "stdafx.h"
#include "Body.h"

Body::Body(float mass, BodyType bodyType) : _mass(mass), _type(bodyType), _appliedForces(DirectX::XMFLOAT3(0,0,0)), 
                                            _currentPoistion(DirectX::XMFLOAT3(0,0,0)), _previousPoistion(DirectX::XMFLOAT3(0, 0, 0)),
                                            _velocity(DirectX::XMFLOAT3(0, 0, 0))
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

DirectX::XMFLOAT3 Body::GetCurrentPosition()
{
    return _currentPoistion;
}

void Body::SetPosition(DirectX::XMFLOAT3 position)
{
    _currentPoistion;
}

DirectX::XMFLOAT3 Body::GetPreviousPosition()
{
    return _previousPoistion;
}

DirectX::XMFLOAT3 Body::GetVelocity()
{
    return _velocity;
}

void Body::SetVelocity(DirectX::XMFLOAT3 velocity)
{
    _velocity = velocity;
}

void Body::ResetAppliedForces()
{
    _appliedForces = DirectX::XMFLOAT3(0, 0, 0);
}

DirectX::XMFLOAT3 Body::GetAppliedForces()
{
    return _appliedForces;
}

void Body::SetAppliedForces(DirectX::XMFLOAT3 force)
{
    _appliedForces = force;
}
