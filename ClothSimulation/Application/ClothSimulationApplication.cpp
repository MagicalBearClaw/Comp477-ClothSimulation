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
    std::string containerTexture = std::filesystem::path("./Assets/Textures/container.jpg").generic_u8string();
    std::cout << "initialize shaders" << std::endl;
    
    moveableSphere = std::make_unique<MoveableSphere>(1.0, glm::vec3(0, 0, 10), 5.2f, containerTexture);
    cloth = std::make_unique<Cloth>(30, 30, containerTexture);

    cloth->AddParticlPositionConstraint(0);
    cloth->AddParticlPositionConstraint(29);
    std::function<void(Particle*)> collisionHandler = std::bind(&MoveableSphere::ClothCollisionHandler, moveableSphere.get(), std::placeholders::_1);
    cloth->AddCollisionHandler(collisionHandler);
    
    // Light VAO
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Some attributes
    //glPointSize(5);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)_windowWith / (float)_windowHeight, 0.1f, 10000.0f);

    GLint object_color_loc = glGetUniformLocation(shaderProgram.ID,
        "object_color");
    GLint light_color_loc = glGetUniformLocation(shaderProgram.ID,
        "light_color");
    GLint light_pos_loc = glGetUniformLocation(shaderProgram.ID,
        "light_pos");

    glUniform3f(object_color_loc, 1.0f, 0.5f, 0.2f);
    glUniform3f(light_color_loc, 1.0f, 1.0f, 1.0f);
    glUniform3f(light_pos_loc, 0.2f, 1.8f, 1.0f);

    //draw_sphere(cloth->get_ball_radius(), cloth->get_ball_center());
    cloth->Draw(shaderProgram, camera, projection);
    moveableSphere->Draw(shaderProgram, camera, projection);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void ClothSimulationApplication::Update(float deltaTime)
{
    ProccessKeyboardInput(deltaTime);
}

void ClothSimulationApplication::FixedUpdate(float deltaTime)
{
    float timestep = 0.00015f;
    cloth->Update(timestep);
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



    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        moveableSphere->Update(MoveableSphere::Direction::FORWARD, deltaTime);
    }

    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        moveableSphere->Update(MoveableSphere::Direction::BACKWARD, deltaTime);
    }

    if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        moveableSphere->Update(MoveableSphere::Direction::LEFT, deltaTime);
    }

    if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        moveableSphere->Update(MoveableSphere::Direction::RIGHT, deltaTime);
    }

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::RIGHT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::Direction::LEFT, deltaTime);
}

