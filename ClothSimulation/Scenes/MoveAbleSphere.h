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
		Up,
		Down,
    };
	MoveableSphere(float radius, glm::vec3 position, float speed, const std::string& textureFileName);
	void Update(Direction direction, float deltaTime);
	void Draw(Shader& shader, Camera& camera, glm::mat4 projection);
	void ClothCollisionHandler(Particle* particle);
public:
	float Radius;
	glm::vec3 Position;
	float Speed;
private:
	bool keys[1024];
	std::unique_ptr<Sphere> sphere;
};
