#pragma once
#include "../stdafx.h"
#include "Projectile.h"

Projectile::Projectile(float radius, glm::vec3 position, float speed) : Radius(radius), Position(position), Speed(speed), CollisionOffset(0.17)
{
	sphere = std::make_unique<Sphere>(50, radius, position);
    IsActive = false;
}

void Projectile::Update(float deltaTime)
{
    if (!IsActive)
        return;

    Position.z -= Speed * deltaTime;
    sphere->Update(Position);
}

void Projectile::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    if (!IsActive)
        return;
	sphere->Draw(shader, camera, Radius, Position, projection);
}

void Projectile::ClothCollisionHandler(Particle* particle)
{
    if (!IsActive)
        return;
    glm::vec3 offset = particle->Position - Position;
    if (glm::length(offset) < Radius + CollisionOffset)
    {
        particle->Position += glm::normalize(offset) * (Radius + CollisionOffset - glm::length(offset));
    }
}
