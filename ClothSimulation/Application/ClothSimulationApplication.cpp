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
        std::cout << "failed to initialize base application" << std::endl;
        return false;
    }
    std::cout << "Initialize base Application" << std::endl;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _imguiIO = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(_glslVersion.c_str());
    std::cout << "Initialized IMGUI" << std::endl;

    _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glEnable(GL_DEPTH_TEST);

    shaderProgram = Shader();
    shaderProgram.Load("./Assets/Shaders/main.vs", "./Assets/Shaders/main.fs");
    std::string containerTexture = std::filesystem::path("./Assets/Textures/unicorn.jpg").generic_u8string();
    std::cout << "initialize shaders" << std::endl;

    light = std::make_unique<Light>();
    light->Color = glm::vec3(1.0f, 1.0f, 1.0f);
    light->Position = glm::vec3(0.2f, 1.8f, 1.0f);


    cloth = std::make_unique<Cloth>(30, 30, containerTexture);

    // Ball initialization
    ballRadius = 0.25f;
    ballCenter = glm::vec3(cloth->SegmentLength * 30 * 0.5f, cloth->SegmentLength * 30 * 1.8f, ballRadius * 2);

    moveableSphere = std::make_unique<MoveableSphere>(ballRadius, ballCenter, .2f, containerTexture);
    cloth->AddParticlPositionConstraint(0);
    cloth->AddParticlPositionConstraint(29);


    verletIntergration = std::make_unique<VerletIntegrator>(0.001f);
    semiEulerIntergration = std::make_unique<SemiImplicitEulerIntegrator>();
    rk4Integrator = std::make_unique<RK4Integrator>();
    gravitationalForce = std::make_unique<GravitationalForce>(glm::vec3(0, 0.98f, 0));
    springForce = std::make_unique<SpringForce>(30, 30, cloth->SegmentLength, cloth->Stiffness);
    windForce = std::make_unique<WindForce>(0.0005f, 0.002f);
    windForce->IsEnabled = false;
    cloth->IntergrationMethod = rk4Integrator.get();

    std::function<void(Particle*)> collisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
    cloth->AddCollisionHandler(collisionHandler);
    cloth->AddForceGenerator(gravitationalForce.get());
    cloth->AddForceGenerator(springForce.get());
    cloth->AddForceGenerator(windForce.get());

    std::cout << "Initialized scnenes, ready to work" << std::endl;
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

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWith / (float)_windowHeight, 0.001f, 10000.0f);

    // Color input
    GLint object_color_loc = glGetUniformLocation(shaderProgram.ID,
        "object_color");


    glUniform3f(object_color_loc, 1.0f, 0.5f, 0.2f);

    light->Draw(shaderProgram);
    // Draw
    shaderProgram.Use();

    cloth->Draw(shaderProgram, camera, projection);
    moveableSphere->Draw(shaderProgram, camera, projection);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ClothSimulationApplication::Update(float deltaTime)
{
    ProccessKeyboardInput(deltaTime);
    if (keys[GLFW_KEY_I]) 
    {
        moveableSphere->Update('I');
    } 
    if (keys[GLFW_KEY_K])
    {
        moveableSphere->Update('K');
    }
    if (keys[GLFW_KEY_J])
    {
        moveableSphere->Update('J');
    }
    if (keys[GLFW_KEY_L])
    {
        moveableSphere->Update('L');
    }
    if (keys[GLFW_KEY_U])
    {
        moveableSphere->Update('U');
    }
    if (keys[GLFW_KEY_O])
    {
        moveableSphere->Update('O');
    }
}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
    float timeStep = (float)1/60.0f;
    //float timeStep = 0.00015f;
    cloth->Update(timeStep, cloth->get_ball_center(), cloth->get_ball_radius());
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

    if (glfwGetKey(_window, GLFW_KEY_I) == GLFW_PRESS)
    {
        keys[GLFW_KEY_I] = true;
    }
    else {
        keys[GLFW_KEY_I] = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_J) == GLFW_PRESS)
    {
        keys[GLFW_KEY_J] = true;
    }
    else {
        keys[GLFW_KEY_J] = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_K) == GLFW_PRESS)
    {
        keys[GLFW_KEY_K] = true;
    }
    else {
        keys[GLFW_KEY_K] = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS)
    {
        keys[GLFW_KEY_L] = true;
    }
    else {
        keys[GLFW_KEY_L] = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_O) == GLFW_PRESS)
    {
        keys[GLFW_KEY_O] = true;
    }
    else {
        keys[GLFW_KEY_O] = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_U) == GLFW_PRESS)
    {
        keys[GLFW_KEY_U] = true;
    }
    else {
        keys[GLFW_KEY_U] = false;
    }



    //if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::FORWARD, deltaTime);
    //}

    //if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::BACKWARD, deltaTime);
    //}

    //if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::LEFT, deltaTime);
    //}

    //if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::RIGHT, deltaTime);
    //}

    //if (glfwGetKey(_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::Up, deltaTime);
    //}

    //if (glfwGetKey(_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    //{
    //    moveableSphere->Update(MoveableSphere::Direction::Down, deltaTime);
    //}

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::RIGHT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::LEFT, deltaTime);
}

