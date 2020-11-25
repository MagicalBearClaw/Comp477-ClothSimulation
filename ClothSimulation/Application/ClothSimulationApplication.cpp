#include "../stdafx.h"
#include "ClothSimulationApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _imguiIO = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(_glslVersion.c_str());

    _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glEnable(GL_DEPTH_TEST);

    cubeMapShader = Shader();
    cubeMapShader.Load("./Assets/Shaders/6.1.cubemaps.vs", "./Assets/Shaders/6.1.cubemaps.fs");
    skyboxShader = Shader();
    skyboxShader.Load("./Assets/Shaders/6.1.skybox.vs", "./Assets/Shaders/6.1.skybox.fs");

    plane = std::make_unique<Plane>(10, 5, 20, 15, std::filesystem::path("./Assets/Textures/container.jpg").generic_u8string());
    sphere = std::make_unique<Sphere>(std::filesystem::path("./Assets/Textures/earth2048.jpg").generic_u8string(), 1);
    skyBox = std::make_unique<SkyBox>();

    integrator = std::make_unique<SemiImplicitEulerIntergrator>();
    cloth = std::make_unique<Cloth>(plane.get());
    cloth->StructualStiffness = 2.0f;
    cloth->StructualDamping = 0.02f;
    cloth->ShearStiffness = 2.0f;
    cloth->ShearDamping = 0.02f;
    cloth->FlexionStiffness = 2.0f;
    cloth->FlexionDamping = 0.02f;
    cloth->IntegrationMethod = integrator.get();
    cloth->Mass = 2.0f;
    cloth->Initialize();
    GravitationalForce* gravity =  new GravitationalForce(glm::vec3(0, -9.81f, 0));
    cloth->AddForceGenerator(gravity);

    //constrain the two top corners of the cloth so that we can control it
    PositionConstraint* topLeftCorner = new PositionConstraint(cloth->Particles[0], cloth->Particles[0]->GetCurrentPosition());
    cloth->AddConstraint(topLeftCorner);
    PositionConstraint* topRightCorner = new PositionConstraint(cloth->Particles[15], cloth->Particles[0]->GetCurrentPosition());
    cloth->AddConstraint(topLeftCorner);


    cubeMapShader.Use();
    cubeMapShader.SetInt("texture1", 0);

    skyboxShader.Use();
    skyboxShader.SetInt("skybox", 0);


    return true;
}

void ClothSimulationApplication::Draw(float deltaTime)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // do imgui Rendering here
    ImGui::Render();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWith / (float)_windowHeight, 0.1f, 100.0f);

    if (drawInWireframe)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        ////plane->Draw(cubeMapShader, camera, projection);
        ////sphere->Draw(cubeMapShader, camera, projection);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);;
    }
    else {
        //sphere->Draw(cubeMapShader, camera, projection);
        cloth->Draw(cubeMapShader, camera, projection);
    }
    // draw skybox
    skyBox->Draw(skyboxShader, camera, projection);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ClothSimulationApplication::Update(float deltaTime)
{
    ProccessKeyboardInput(deltaTime);
}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
    cloth->Update(deltaTime);
}

void ClothSimulationApplication::ShutDOwn()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ClothSimulationApplication::HandleMouseScroll(double xoffset, double yoffset)
{
    if (_imguiIO->WantCaptureMouse)
    {
        return;
    }
    camera.ProcessMouseScroll(yoffset);
}

void ClothSimulationApplication::HandleMouse(double xPosition, double yPosition)
{
    if (_imguiIO->WantCaptureMouse)
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
    float yoffset = lastY - yPosition; // reversed since y-coordinates go from bottom to top

    lastX = xPosition;
    lastY = yPosition;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ClothSimulationApplication::ProccessKeyboardInput(float deltaTime)
{
    if (_imguiIO->WantCaptureKeyboard)
    {
        return;
    }

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_PRESS)
    {
        drawInWireframe = !drawInWireframe;
    }

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::LEFT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::RIGHT, deltaTime);
}

