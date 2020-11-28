#pragma once
#include "../stdafx.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "application.h"

#include "../Rendering//Camera.h"
#include "../Rendering/Shader.h"
#include "../Physics//cloth.h"
#include "../Physics/Integrators/IIntergrator.h"
#include "../Physics/Integrators/VerletIntegrator.h"
#include "../Physics/Integrators/SemiImplicitEulerIntegrator.h"
#include "../Physics/Integrators/ExplicitEulerIntergrator.h"

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
	void draw_sphere(Shader& shader, Camera& camera, glm::mat4 projection, glm::vec3 position);
	void draw_sphere(float r, glm::vec3 c);
private:
	Camera camera;
	Shader shaderProgram;
	float lastX;
	float lastY;
	bool firstMouse;
	bool drawInWireframe;
	bool showImguiWindow;
	ImGuiIO* _imguiIO;
	ImVec4 _clearColor;

	std::unique_ptr<VerletIntergrator> verletIntergration;
	std::unique_ptr<SemiImplicitEulerIntergrator> semiEulerIntergration;

	std::unique_ptr<GravitationalForce> gravitationalForce;
	std::unique_ptr<SpringForce> springForce;
	std::unique_ptr<WindForce> windForce;

	bool drawSphere;
	GLuint lightVAO;
	int m;
	std::vector<GLfloat> vertices;
	std::vector<int> indices;
	std::unique_ptr<Cloth> cloth;
	std::unique_ptr<MoveableSphere> moveableSphere;
	bool keys[1024];
};