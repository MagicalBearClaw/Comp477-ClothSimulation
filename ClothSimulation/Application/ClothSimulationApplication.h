#pragma once
#include "../stdafx.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "application.h"

#include "../Rendering//Camera.h"
#include "../Rendering/Shader.h"

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

	void ProccessKeyboardInput(float deltaTime);

private:
	unsigned int LoadTexture(const char* path);
	unsigned int LoadCubemap(std::vector<std::string> faces);

private:
	// camera
	Camera camera;
	Shader cubeMapShader;
	Shader skyboxShader;
	float lastX;
	float lastY;
	bool firstMouse;
	unsigned int cubeVAO;
	unsigned int cubeVBO;

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	unsigned int cubeTexture;
	unsigned int cubemapTexture;

	float cubeVertices[180];
	float skyboxVertices[108];
	bool showImguiWindow;
	ImGuiIO* _imguiIO;
	ImVec4 _clearColor;
};