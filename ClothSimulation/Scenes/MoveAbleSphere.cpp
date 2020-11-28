#include "MoveAbleSphere.h"

MoveableSphere::MoveableSphere(float radius, glm::vec3 position, float speed, const std::string& textureFileName) : Radius(radius), Position(position), Speed(speed)
{
    sphere = std::make_unique<Sphere>(textureFileName, radius);
}


void MoveableSphere::Update(Direction direction, float deltaTime)
{
    float speed = 0.05f;
    if (direction == Direction::FORWARD)
        Position.z -= speed;
    if (direction == Direction::BACKWARD)
        Position.z += speed;
    if (direction == Direction::LEFT)
        Position.x += speed;
    if (direction == Direction::RIGHT)
        Position.x -= speed;
    if (direction == Direction::Up)
        Position.y -= speed;
    if (direction == Direction::Down)
        Position.y += speed;
}

void MoveableSphere::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    sphere->Draw(shader, camera, model, projection);
}

void MoveableSphere::ClothCollisionHandler(Particle* particle)
{
    glm::vec3 offset = particle->Position - Position;
    if (glm::length(offset) < Radius)
    {
        particle->Position += glm::normalize(offset) * (Radius - glm::length(offset));
    }
}
