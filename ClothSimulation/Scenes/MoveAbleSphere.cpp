#include "MoveAbleSphere.h"

MoveableSphere::MoveableSphere(float radius, glm::vec3 position, float speed, const std::string& textureFileName) : Radius(radius), Position(position), Speed(speed)
{
    sphere = std::make_unique<Sphere>(textureFileName, radius);
}


void MoveableSphere::Update(Direction direction, float deltaTime)
{
    float velocity = Speed * deltaTime;
    if (direction == Direction::FORWARD)
        Position.z -= velocity;
    if (direction == Direction::BACKWARD)
        Position.z += velocity;
    if (direction == Direction::LEFT)
        Position.x += velocity;
    if (direction == Direction::RIGHT)
        Position.x -= velocity;
}

void MoveableSphere::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.05f));
    model = glm::translate(model, Position);
    sphere->Draw(shader, camera, model, projection);
}

void MoveableSphere::ClothCollisionHandler(Particle* particle)
{
    glm::vec3 offset = particle->CurrentPosition - Position;
    if (glm::length(offset) < Radius)
    {
        particle->CurrentPosition += glm::normalize(offset) * (Radius - glm::length(offset));
    }
}
