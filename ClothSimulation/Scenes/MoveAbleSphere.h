#include "../stdafx.h"
#include "../Rendering/Sphere.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Shader.h"
#include "../Physics/Particle.h"

class MoveableSphere
{
public:
	enum class Direction 
	{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
		UP,
		DOWN,
    };

	MoveableSphere(float radius, glm::vec3 position, float speed);
	void Update(Direction direction, float deltaTime);
	void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
	void ClothCollisionHandler(Particle* particle);
public:
	float Radius;
	float Speed;
	float CollisionOffset;
	glm::vec3 Position;
private:
	std::unique_ptr<Sphere> sphere;
};
