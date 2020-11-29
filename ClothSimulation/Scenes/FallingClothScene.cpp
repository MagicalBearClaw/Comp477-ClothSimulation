#include "../stdafx.h"
#include "FallingClothScene.h"

FallingClothScene::FallingClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight)
									 : Scene(windowTitle, applicationWindowWidth, applicationWindowHeight)
{
	Initialize();
}

void FallingClothScene::Initialize()
{
	moveableSphere = std::make_unique<MoveableSphere>(ballRadius, ballPosition, 0.012f);
	std::function<void(Particle*)> collisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
	cloth->AddCollisionHandler(collisionHandler);
}

void FallingClothScene::Update(bool keyState[], float deltaTime)
{
	Scene::Update(keyState, deltaTime);
	moveableSphere->CollisionOffset = CurrentCollisionOffset;
	cloth->Update(CurrentTimeStep);
}

void FallingClothScene::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{

}