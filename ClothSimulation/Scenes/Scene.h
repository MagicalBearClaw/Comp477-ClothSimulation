#pragma once
#include "../stdafx.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


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
	Scene(const std::string& windowTitle, int applicationWindowWidth, int applicationWindowHeight);
	virtual void Initialize();
	virtual void Update(bool keyState[], float deltaTime);
	virtual void Draw(Shader& shader, Camera& camera, glm::mat4 projection) = 0;
	void DrawUI(float deltaTime);

public:
	bool DrawInWireFrame;
protected:
	virtual void Restart() = 0;
	virtual void Reset() = 0;
protected:
	enum IntegratorType
	{
		ExplicitEuler,
		Verlet,
		SemiImplicitEuler,
		RK4
	};

	// wind force properties
	glm::vec2 WindSpeed;
	glm::vec2 DefaultWindSpeed;
	bool IsWindEnabled;

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
	glm::vec3 ClothColor;
	glm::vec3 DefaultClothColor;

	glm::vec2 ClothSize;
	glm::vec2 DefaultClothSize;

	float SegmentLength;
	float DefaultSegmentLength;

	int NumberOfConstraintIterations;
	int DefaultNumberOfConstraintIterations;
	float Stiffness;
	float DefaultStiffness;
	float Mass;
	float DefaultMass;

	bool IsSimulationUIOpen;

	float CurrentTimeStep;
	float CurrentCollisionOffset;

protected:
	std::unique_ptr<Light> light;

	std::unique_ptr<ExplicitEulerIntegrator> explicitEulerInegrator;
	std::unique_ptr<SemiImplicitEulerIntegrator> semiEulerInegrator;
	std::unique_ptr<VerletIntegrator> verletInegrator;
	std::unique_ptr<RK4Integrator> rk4Integrator;

	std::unique_ptr<GravitationalForce> gravitationalForce;
	std::unique_ptr<SpringForce> springForce;
	std::unique_ptr<WindForce> windForce;

	float ballRadius;
	glm::vec3 ballPosition;


	std::deque<float> deltaTimes;
	const int maxDeltaTimes = 1000;
	
	std::unique_ptr<Cloth> cloth;

	int applicationWindowWidth;
	int applicationWindowHeight;

	int integrationMethodType;
	const char* intergrationMethodNames[4] = { "Explicit Euler", "Verlet", "Semi Implicit Euler", "RK4" };
	
	std::string windowTitle;
};