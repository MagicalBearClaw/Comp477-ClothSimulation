#include "../stdafx.h"
#include "ClothSimulationApplication.h"


ClothSimulationApplication::ClothSimulationApplication(const std::string& windowTitle, int windowWith, int windowHeight)
    : Application(windowTitle, windowWith, windowHeight), camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
    float lastX = (float)_windowWith / 2.0;
    float lastY = (float)_windowHeight / 2.0;
    firstMouse = true;
}

bool ClothSimulationApplication::Initialize()
{
    if (!Application::Initialize())
    {
        std::cout << "failed to initialize base application" << std::endl;
        return false;
    }

    std::cout << "Initialize base Application" << std::endl;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imguiIO = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(_glslVersion.c_str());

    std::cout << "Initialized IMGUI" << std::endl;

    clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glEnable(GL_DEPTH_TEST);

    shaderProgram = Shader();
    shaderProgram.Load("./Assets/Shaders/main.vs", "./Assets/Shaders/main.fs");
    std::cout << "initialize shaders" << std::endl;

    basicClothScene = std::make_unique<BasicClothScene>("Cloth Simulation tools", _windowWith, _windowHeight);
    //fallingClothScene = std::make_unique<FallingClothScene>("Cloth Simulation tools", _windowWith, _windowHeight);
    std::cout << "Initialized scnenes, ready to work" << std::endl;
    return true;
}

void ClothSimulationApplication::Draw(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    basicClothScene->DrawUI(deltaTime);
    ImGui::Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWith / (float)_windowHeight, 0.001f, 10000.0f);
    shaderProgram.Use();
    basicClothScene->Draw(shaderProgram, camera, projection);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ClothSimulationApplication::Update(float deltaTime)
{
    ProccessKeyboardInput(deltaTime);
    basicClothScene->Update(keys, deltaTime);
}

void ClothSimulationApplication::ShutDOwn()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ClothSimulationApplication::HandleMouseScroll(double xoffset, double yoffset)
{
    if (imguiIO->WantCaptureMouse)
    {
        return;
    }

    camera.ProcessMouseScroll(yoffset);
}

void ClothSimulationApplication::HandleMouse(double xPosition, double yPosition)
{
    if (imguiIO->WantCaptureMouse)
    {
        return;
    }

    if (firstMouse)
    {
        lastX = xPosition;
        lastY = yPosition;
        firstMouse = false;
    }

    float xoffset = xPosition - lastX;
    float yoffset = lastY - yPosition;

    lastX = xPosition;
    lastY = yPosition;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ClothSimulationApplication::ProccessKeyboardInput(float deltaTime)
{
    if (imguiIO->WantCaptureKeyboard)
    {
        return;
    }

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
    }

    if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        keys[GLFW_KEY_F1] = true;
    }
    else
    {
        keys[GLFW_KEY_F1] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        keys[GLFW_KEY_UP] = true;
    }
    else
    {
        keys[GLFW_KEY_UP] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        keys[GLFW_KEY_DOWN] = true;
    }
    else
    {
        keys[GLFW_KEY_DOWN] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        keys[GLFW_KEY_LEFT] = true;
    }
    else
    {
        keys[GLFW_KEY_LEFT] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        keys[GLFW_KEY_RIGHT] = true;
    }
    else
    {
        keys[GLFW_KEY_RIGHT] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        keys[GLFW_KEY_RIGHT_SHIFT] = true;
    }
    else
    {
        keys[GLFW_KEY_RIGHT_SHIFT] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        keys[GLFW_KEY_RIGHT_CONTROL] = true;
    }
    else
    {
        keys[GLFW_KEY_RIGHT_CONTROL] = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Direction::FORWARD, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Direction::BACKWARD, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Direction::RIGHT, deltaTime);
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Direction::LEFT, deltaTime);
    }
}

