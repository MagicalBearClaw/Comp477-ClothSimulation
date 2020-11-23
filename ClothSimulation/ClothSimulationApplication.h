#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_dx11.h"

#include "application.h"
#include "Camera.h"
#include "model.h"
#include "Light.h"
#include "render_texture.h"
#include "depth_shader.h"
#include "shadow_shader.h"
#include "soft_shadow_shader.h"
#include "horizontal_blur.h"
#include "vertical_blur.h"
#include "orthographic_window.h"
#include "texture_shader.h"

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
	void RenderSceneToTexture();
	void RenderBlackAndWhiteShadows();
	void DownSampleTexture();
	void RenderHorizontalBlurToTexture();
	void RenderVerticalBlurToTexture();
	void UpSampleTexture();
	void Render();

private:
	ImGuiIO _imguiIO;
	ImVec4 _clearColor;
	std::unique_ptr<Camera> _camera;
	std::unique_ptr<Model> _cubeModel;
	std::unique_ptr<Model> _sphereModel;
	std::unique_ptr<Model> _floorModel;
	std::unique_ptr<Light> _light;
	std::unique_ptr<RenderTexture> _renderTexture;
	std::unique_ptr<DepthShader> _depthShader;
	std::unique_ptr<RenderTexture> _blackWhiteRenderTexture;
	std::unique_ptr<ShadowShader> _shadowShader;
	std::unique_ptr<RenderTexture> _downSampleTexture;
	std::unique_ptr<SoftShadowShader> _softShadowShader;
	std::unique_ptr<HorizontalBlurShader> _horizontalBlurShader;
	std::unique_ptr<VerticalBlurShader> _verticalBlurShader;
	std::unique_ptr<OrthographicWindow> _orthographicWindow;
	std::unique_ptr<TextureShader> _textureShader;
	std::unique_ptr<RenderTexture> _horizontalBlurTexture;
	std::unique_ptr<RenderTexture> _verticalBlurTexture;
	std::unique_ptr<RenderTexture> _upSampleTexure;
	std::unique_ptr<OrthographicWindow> _fullScreenOrthographicWindow;

	const float SCREEN_DEPTH = 100.0f;
	const float SCREEN_NEAR = 1.0f;
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;

};