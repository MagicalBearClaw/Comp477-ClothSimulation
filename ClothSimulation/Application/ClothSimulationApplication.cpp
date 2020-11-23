#include "../stdafx.h"
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(_glslVersion.c_str());


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
        //_renderContext->CleanupRenderTarget();
        //_renderContext->GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
        //_renderContext->CreateRenderTarget();
    }

    return false;
}

void ClothSimulationApplication::Draw(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Rendering
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window);
}

void ClothSimulationApplication::Update(float deltaTime)
{
}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
}

void ClothSimulationApplication::ShutDOwn()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
