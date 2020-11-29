#include "MoveAbleSphere.h"

MoveableSphere::MoveableSphere(float radius, glm::vec3 position, float speed = 0.02f) : Radius(radius), Position(position), Speed(speed)
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
    if (direction == Direction::Up)
        Position.y -= Speed;
    if (direction == Direction::Down)
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
    if (glm::length(offset) < Radius + 0.17f)
    {
        particle->Position += glm::normalize(offset) * (Radius + 0.17f - glm::length(offset));
    }
}
