#include "MoveAbleSphere.h"

MoveableSphere::MoveableSphere(float radius, glm::vec3 position, float speed = 0.02f) 
                               : Radius(radius), Position(position), Speed(speed), CollisionOffset(0.17)
{
    sphere = std::make_unique<Sphere>(50, radius, position);
}

void MoveableSphere::Update(Direction direction, float deltaTime)
{
    if (direction == Direction::FORWARD)
        Position.z -= Speed;
    if (direction == Direction::BACKWARD)
        Position.z += Speed;
    if (direction == Direction::LEFT)
        Position.x += Speed;
    if (direction == Direction::RIGHT)
        Position.x -= Speed;
    if (direction == Direction::UP)
        Position.y -= Speed;
    if (direction == Direction::DOWN)
        Position.y += Speed;

    sphere->Update(Position);
}

void MoveableSphere::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    sphere->Draw(shader, camera, Radius, Position);
}

void MoveableSphere::ClothCollisionHandler(Particle* particle)
{
    glm::vec3 offset = particle->Position - Position;
    if (glm::length(offset) < Radius + CollisionOffset)
    {
        particle->Position += glm::normalize(offset) * (Radius + CollisionOffset - glm::length(offset));
    }
}
