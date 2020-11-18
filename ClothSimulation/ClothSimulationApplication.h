#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_dx11.h"

#include "application.h"

class ClothSimulationApplication : public Application
{
public:
	ClothSimulationApplication(const std::string& windowTitle, int windowWith, int windowHeight);
	virtual bool Initialize() override;
	virtual bool HandleWindowEvent(SDL_Event& event) override;
	virtual void Draw(float deltaTime) override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float deltaTime) override;

private:
	ImGuiIO _imguiIO;
	ImVec4 _clearColor;
};