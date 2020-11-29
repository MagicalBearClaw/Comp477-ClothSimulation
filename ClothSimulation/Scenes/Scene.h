#pragma once
#include "../stdafx.h"
#include "../Rendering/Light.h"
#include "../Physics//cloth.h"
#include "../Physics/Integrators/IIntergrator.h"
#include "../Physics/Integrators/VerletIntegrator.h"
#include "../Physics/Integrators/SemiImplicitEulerIntegrator.h"
#include "../Physics/Integrators/ExplicitEulerIntegrator.h"
#include "../Physics/Integrators/RK4Integrator.h"
#include "../Physics/ForceGenerators/GravitationalForce.h"
#include "../Physics/ForceGenerators/WindForce.h"
#include "../Physics/ForceGenerators/SpringForce.h"

class Scene
{
public:
	virtual void Initialize() = 0;
	virtual void FixedUpdate(float deltaTime) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	void DrawUI();


protected:
	virtual void Reset();
protected:
	enum class IntegratorType
	{
		ExplicitEuler,
		Verlet,
		SemiImplicitEuler,
		RK4
	};

	// wind force properties
	float MinWindSpeed;
	float MaxWindSpeed;
	float DefaultMinWindSpeed;
	float DefaultMaxWindSpeed;
	// gravity force properties
	glm::vec3 Gravity;
	glm::vec3 DefaultGravity;

	// light properties
	glm::vec3 LightColor;
	glm::vec3 LightPosition;

	glm::vec3 DefaultLightColor;
	glm::vec3 DefaultLightPosition;
	
	// explicit eurler integrator poperties
	float ExplicitEulerTimeStep;
	float ExplicitEulerCollisionOffset;

	float DefaultExplicitEulerTimeStep;
	float DefaultExplicitEulerCollisionOffset;

	// verlet integrator poperties
	float VerletDrag;
	float VerletTimeStep;
	float VerletCollisionOffset;

	float DefaultVerletDrag;
	float DefaultVerletTimeStep;
	float DefaultVerletCollisionOffset;


	// semi implicit integrator poperties
	float SemiImplicitTimeStep;
	float SemiImplicitCollisionOffset;

	float DefaultSemiImplicitTimeStep;
	float DefaultSemiImplicitCollisionOffset;

	// rk4 integrator poperties
	float Rk4TimeStep;
	float Rk4tCollisionOffset;

	float DefaultRk4TimeStep;
	float DefaultRk4tCollisionOffset;

	// cloth properties
	glm::vec3 Color;
	glm::vec3 DefaultColor;

	int Width;
	int Height;
	int NumberOfConstraintIterations;
	int Stiffness;
	int Mass;

	int DefaultWidth;
	int DefaultHeight;
	int DefaultNumberOfConstraintIterations;
	int DefaultStiffness;
	int DefaultMass;

protected:
	std::unique_ptr<Light> light;

	std::unique_ptr<ExplicitEulerIntegrator> explicitEulerInegrator;
	std::unique_ptr<SemiImplicitEulerIntegrator> semiEulerInegrator;
	std::unique_ptr<VerletIntegrator> verletInegrator;
	std::unique_ptr<RK4Integrator> rk4Integrator;

	std::unique_ptr<GravitationalForce> gravitationalForce;
	std::unique_ptr<SpringForce> springForce;
	std::unique_ptr<WindForce> windForce;

	std::unique_ptr<Cloth> cloth;
};