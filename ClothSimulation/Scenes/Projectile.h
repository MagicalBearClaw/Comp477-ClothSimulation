#pragma once
#include "../stdafx.h"
#include "../Rendering/Sphere.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Shader.h"
#include "../Physics/Particle.h"

class Projectile
{
public:
	Projectile(float radius, glm::vec3 position, float speed);
	void Update(float deltaTime);
	void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
	void ClothCollisionHandler(Particle* particle);
public:
	float Radius;
	float Speed;
	float CollisionOffset;
	bool IsActive;
	glm::vec3 Position;
	glm::vec3 Velocity;
private:
	std::unique_ptr<Sphere> sphere;
};
