#include "../stdafx.h"
#include "FallingClothScene.h"

FallingClothScene::FallingClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight)
									 : Scene(windowTitle, applicationWindowWidth, applicationWindowHeight)
{
	ballPosition = glm::vec3(0.5, 1, 0.5);
	Initialize();
}

void FallingClothScene::Initialize()
{

	moveableSphere = std::make_unique<MoveableSphere>(ballRadius, ballPosition, 0.012f);
	catTexturePath = std::filesystem::path("./Assets/Textures/cat2.jpg").generic_u8string();
	cloth = std::make_unique<Cloth>(ClothSize.x, ClothSize.y, catTexturePath);
	RecreateCloth();
}

void FallingClothScene::Update(bool keyState[], float deltaTime)
{
	Scene::Update(keyState, deltaTime);
	moveableSphere->CollisionOffset = CurrentCollisionOffset;
	cloth->Update(CurrentTimeStep);
}

void FallingClothScene::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
	light->Draw(shader);
	moveableSphere->Draw(shader, camera, projection);
	cloth->Draw(shader, camera, projection);
}

void FallingClothScene::RecreateCloth()
{
	if (cloth != nullptr)
	{
		cloth.reset();
	}

	cloth = std::make_unique<Cloth>(ClothSize.x, ClothSize.y, catTexturePath);
	cloth->Mass = Mass;
	cloth->Color = ClothColor;
	cloth->NumberOfConstraintIterations = NumberOfConstraintIterations;
	springForce = std::make_unique<SpringForce>(ClothSize.x, ClothSize.y, SegmentLength, Stiffness, Damping);
	switch (integrationMethodType)
	{
		case IntegratorType::Verlet:
		{
			cloth->IntergrationMethod = verletInegrator.get();
			break;
		}
		case IntegratorType::ExplicitEuler:
		{
			cloth->IntergrationMethod = explicitEulerInegrator.get();
			break;
		}
		case IntegratorType::SemiImplicitEuler:
		{
			cloth->IntergrationMethod = semiEulerInegrator.get();
			break;
		}
		case IntegratorType::RK4:
		{
			cloth->IntergrationMethod = rk4Integrator.get();
			break;
		}
	}

	cloth->AddForceGenerator(gravitationalForce.get());
	cloth->AddForceGenerator(springForce.get());
	cloth->AddForceGenerator(windForce.get());
	std::function<void(Particle*)> collisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
	cloth->AddCollisionHandler(collisionHandler);
}