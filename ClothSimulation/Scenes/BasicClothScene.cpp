#include "../stdafx.h"
#include "BasicClothScene.h"

BasicClothScene::BasicClothScene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight) 
								: Scene(windowTitle, applicationWindowWidth, applicationWindowHeight)
{
	DefaultProjectilePosition = glm::vec3(0.5f, 0.9f, 5.7f);
	shootInterval = 2.0f;
	Initialize();
}

void BasicClothScene::Initialize()
{
	moveableSphere = std::make_unique<MoveableSphere>(ballRadius, ballPosition, 0.012f);
	projectile = std::make_unique<Projectile>(ballRadius, DefaultProjectilePosition, 5.12f);
	catTexturePath = std::filesystem::path("./Assets/Textures/cat2.jpg").generic_u8string();
	RecreateCloth();
}

void BasicClothScene::Update(bool keyState[], Camera& camera, float deltaTime)
{
	Scene::Update(keyState, camera, deltaTime);

	moveableSphere->CollisionOffset = CurrentCollisionOffset;
	projectile->CollisionOffset = CurrentCollisionOffset;

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

	
	if (keyState[GLFW_KEY_SPACE] &&!hasFired)
	{
		projectile->IsActive = true;
		hasFired = true;
		shootStartedTime = glfwGetTime();
		projectile->Position = camera.Position;
		projectile->CameraFront = camera.Front;
	}

	int shotDeltaTime = glfwGetTime() - shootStartedTime;
	if (hasFired && shotDeltaTime > shootInterval)
	{
		projectile->IsActive = false;
		hasFired = false;
		projectile->Position = camera.Position;
		projectile->CameraFront = camera.Front;
		projectile->Velocity = glm::vec3(0, 0, 0);
		shootStartedTime = 0;
	}

	projectile->Update(deltaTime);
	cloth->Update(CurrentTimeStep);
}

void BasicClothScene::Draw(Shader& shader, Camera& camera, glm::mat4 projection)
{
	light->Draw(shader);
	moveableSphere->Draw(shader, camera, projection);
	projectile->Draw(shader, camera, projection);
	cloth->Draw(shader, camera, projection);
}



void BasicClothScene::RecreateCloth()
{
	if (cloth != nullptr)
	{
		cloth.reset();
	}

	if (springForce != nullptr)
	{
		springForce.reset();
	}

	SegmentLength = CalculateSegmentLength(ClothResolution);
	cloth = std::make_unique<Cloth>(ClothResolution.x, ClothResolution.y, SegmentLength, Mass, catTexturePath);
	cloth->Mass = Mass;
	cloth->Color = ClothColor;
	cloth->NumberOfConstraintIterations = NumberOfConstraintIterations;
	springForce = std::make_unique<SpringForce>(ClothResolution.x, ClothResolution.y, SegmentLength, Stiffness, Damping);
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
	cloth->AddParticlPositionConstraint(0);
	cloth->AddParticlPositionConstraint(ClothResolution.x - 1);
	std::function<void(Particle*)> moveableSphereCollisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
	std::function<void(Particle*)> projectileCollisionHandler = std::bind(&Projectile::ClothCollisionHandler, projectile.get(), std::placeholders::_1);
	cloth->AddCollisionHandler(moveableSphereCollisionHandler);
	cloth->AddCollisionHandler(projectileCollisionHandler);
}