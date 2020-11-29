#pragma once
#include "../stdafx.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "application.h"

#include "../Rendering//Camera.h"
#include "../Rendering/Shader.h"
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

#include "../Scenes/MoveAbleSphere.h"

class ClothSimulationApplication : public Application
{
public:
	ClothSimulationApplication(const std::string& windowTitle, int windowWith, int windowHeight);

	// Inherited via Application
	virtual bool Initialize() override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void ShutDOwn() override;
	virtual void HandleMouseScroll(double xoffset, double yoffset) override;
	virtual void HandleMouse(double xPosition, double yPosition) override;

private:
	void ProccessKeyboardInput(float deltaTime);
private:
	Camera camera;
	
	Shader shaderProgram;
	
	float lastX;
	float lastY;
	bool firstMouse;
	
	bool drawInWireframe;
	bool showImguiWindow;

	ImGuiIO* imguiIO;
	ImVec4 clearColor;

	// todo move these into the scene;
	glm::vec3 ballCenter;
	float ballRadius;

	std::unique_ptr<Light> light;

	std::unique_ptr<VerletIntegrator> verletIntergration;
	std::unique_ptr<SemiImplicitEulerIntegrator> semiEulerIntergration;
	std::unique_ptr<RK4Integrator> rk4Integrator;
	std::unique_ptr<GravitationalForce> gravitationalForce;
	std::unique_ptr<SpringForce> springForce;
	std::unique_ptr<WindForce> windForce;

	std::unique_ptr<Cloth> cloth;
	std::unique_ptr<MoveableSphere> moveableSphere;
	bool keys[1024];
};