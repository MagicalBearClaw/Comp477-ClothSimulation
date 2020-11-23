#include "../../stdafx.h"
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
    glm::vec3 bodydAPosition = _bodyA->GetCurrentPosition();
    glm::vec3 bodydBPosition = _bodyB->GetCurrentPosition();
    glm::vec3 direction = bodydBPosition - bodydAPosition;
    float currentLength = direction.length();

    if (direction == glm::vec3(0, 0, 0))
    {
        return;
    }

    glm::vec3 normalizedDirection = glm::normalize(direction);
    float scaleFactor = 0.5f * (float)(currentLength - _length);
    glm::vec3 offsetVector = normalizedDirection * scaleFactor;
    glm::vec3 bodyAresultingPosition = bodydAPosition + offsetVector;
    _bodyA->SetPosition(bodyAresultingPosition);

    glm::vec3 bodyBresultingPosition = bodydAPosition + (-offsetVector);
    _bodyB->SetPosition(bodyBresultingPosition);
}
