#include "MoveAbleSphere.h"

MoveableSphere::MoveableSphere(float radius, glm::vec3 position, float speed = 0.02f) : Radius(radius), Position(position), Speed(speed)
{
    sphere = std::make_unique<Sphere>(50, radius, position);

}

void MoveableSphere::Update(char input)
{
    //switch (input) {
    //case 'I':
    //    Position -= glm::vec3(0, speed, 0);
    //    break;
    //case 'K':
    //    Position += glm::vec3(0, speed, 0);
    //    break;
    //case 'J':
    //    Position -= glm::vec3(speed, 0, 0);
    //    break;
    //case 'L':
    //    Position += glm::vec3(speed, 0, 0);
    //    break;
    //case 'U':
    //    Position -= glm::vec3(0, 0, speed);
    //    break;
    //case 'O':
    //    Position += glm::vec3(0, 0, speed);
    //    break;
    //case '[':
    //    if (Radius > 0.2f) Radius -= 0.002f;
    //    break;
    //case ']':
    //    if (Radius < 4.0f) Radius += 0.002f;
    //    break;
    //}

    //sphere->Update(Position);
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
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::scale(model, glm::vec3(0.25f));
    //model = glm::translate(model, Position);
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
