#pragma once
#include "../stdafx.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "application.h"

#include "../Rendering//Camera.h"
#include "../Rendering/Shader.h"
#include "../Physics//cloth.h"
#include "../Rendering/SkyBox.h"

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
	ImGuiIO* _imguiIO;
	ImVec4 _clearColor;

	GLuint lightVAO;
	std::unique_ptr<Cloth> cloth;
	std::unique_ptr<MoveableSphere> moveableSphere;
	bool keys[1024];
};