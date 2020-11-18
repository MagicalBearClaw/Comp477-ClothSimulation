#include "stdafx.h"
#include "ClothSimulationApplication.h"

ClothSimulationApplication::ClothSimulationApplication(const std::string& windowTitle, int windowWith, int windowHeight)
    : Application(windowTitle, windowWith, windowHeight)
{
}

bool ClothSimulationApplication::Initialize()
{
    if (!Application::Initialize())
    {
        return false;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _imguiIO = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForD3D(_window);
    ImGui_ImplDX11_Init(_renderContext->GetDevice(), _renderContext->GetDeviceContext());

    _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    return true;
}

bool ClothSimulationApplication::HandleWindowEvent(SDL_Event& event)
{

    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        return true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window))
        return true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(_window))
    {
        // Release all outstanding references to the swap chain's buffers before resizing.
        _renderContext->CleanupRenderTarget();
        _renderContext->GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
        _renderContext->CreateRenderTarget();
    }

    return false;
}

void ClothSimulationApplication::Draw(float deltaTime)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    // Rendering
    ImGui::Render();

    ID3D11RenderTargetView* mainView = _renderContext->GetMainRenderTargetView();
    _renderContext->GetDeviceContext()->OMSetRenderTargets(1, &mainView, nullptr);
    _renderContext->GetDeviceContext()->ClearRenderTargetView(mainView, (float*)&_clearColor);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    _renderContext->Present();
}

void ClothSimulationApplication::Update(float deltaTime)
{
}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
}
