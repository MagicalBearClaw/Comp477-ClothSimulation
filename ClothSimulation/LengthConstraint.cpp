#include "stdafx.h"
#include "LengthConstraint.h"

LengthConstraint::LengthConstraint(Body* bodyA, Body* bodyB, float length) : _bodyA(bodyA), _bodyB(bodyB), _length(length)
{
}

float LengthConstraint::GetLength()
{
    return 0.0f;
}

void LengthConstraint::SatisfyConstraint()
{
    DirectX::XMVECTOR bodyAPosition = DirectX::XMLoadFloat3(&_bodyA->GetCurrentPosition());
    DirectX::XMVECTOR bodyBPosition = DirectX::XMLoadFloat3(&_bodyB->GetCurrentPosition());
    DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(bodyBPosition, bodyAPosition);
    float currentLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(direction));

    if (isVector3AllZeros(direction))
    {
        return;
    }

    DirectX::XMVECTOR normalizedDirection = DirectX::XMVector3Normalize(direction);

    float scaleFactor = 0.5f * (float)(currentLength - _length);

    DirectX::XMVECTOR offsetVector = DirectX::XMVectorScale(normalizedDirection, scaleFactor);
    DirectX::XMFLOAT3 bodyAresultingPosition(0, 0, 0);
    DirectX::XMStoreFloat3(&bodyAresultingPosition, DirectX::XMVectorAdd(bodyAPosition, offsetVector));
    _bodyA->SetPosition(bodyAresultingPosition);
    DirectX::XMFLOAT3 bodyBresultingPosition(0, 0, 0);
    DirectX::XMStoreFloat3(&bodyBresultingPosition, DirectX::XMVectorAdd(bodyBPosition, DirectX::XMVectorNegate(offsetVector)));
    _bodyB->SetPosition(bodyBresultingPosition);
}
