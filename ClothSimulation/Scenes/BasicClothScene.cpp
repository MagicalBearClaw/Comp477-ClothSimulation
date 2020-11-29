#include "../stdafx.h"
#include "BasicClothScene.h"

BasicClothScene::BasicClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight) 
								: Scene(windowTitle, applicationWindowWidth, applicationWindowHeight)
{
	Initialize();
}

void BasicClothScene::Initialize()
{
	moveableSphere = std::make_unique<MoveableSphere>(ballRadius, ballPosition, 0.012f);

	cloth->AddParticlPositionConstraint(0);
	cloth->AddParticlPositionConstraint(ClothSize.x -1);
	std::function<void(Particle*)> collisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
	cloth->AddCollisionHandler(collisionHandler);
}

void BasicClothScene::Update(bool keyState[], float deltaTime)
{
	Scene::Update(keyState, deltaTime);

	moveableSphere->CollisionOffset = CurrentCollisionOffset;

	if (keyState[GLFW_KEY_UP])
	{
		moveableSphere->Update(MoveableSphere::Direction::FORWARD, deltaTime);
	}
	if (keyState[GLFW_KEY_DOWN])
	{
		moveableSphere->Update(MoveableSphere::Direction::BACKWARD, deltaTime);
	}
	if (keyState[GLFW_KEY_LEFT])
	{
		moveableSphere->Update(MoveableSphere::Direction::LEFT, deltaTime);
	}
	if (keyState[GLFW_KEY_RIGHT])
	{
		moveableSphere->Update(MoveableSphere::Direction::RIGHT, deltaTime);
	}
	if (keyState[GLFW_KEY_RIGHT_SHIFT])
	{
		moveableSphere->Update(MoveableSphere::Direction::UP, deltaTime);
	}
	if (keyState[GLFW_KEY_RIGHT_CONTROL])
	{
		moveableSphere->Update(MoveableSphere::Direction::DOWN, deltaTime);
	}

	cloth->Update(CurrentTimeStep);
}

void BasicClothScene::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
	light->Draw(shader);
	moveableSphere->Draw(shader, camera, projection);
	cloth->Draw(shader, camera, projection);
}
