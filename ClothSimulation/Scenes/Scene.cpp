#include "../stdafx.h"
#include "Scene.h"

Scene::Scene(const std::string& windowTitle , int applicationWindowWidth, int applicationWindowHeight) 
            : windowTitle(windowTitle), applicationWindowWidth(applicationWindowWidth), applicationWindowHeight(applicationWindowHeight)
{
    LightColor = DefaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    LightPosition = DefaultLightPosition = glm::vec3(0.2f, 1.8f, 1.0f);

    DrawInWireFrame = false;
    IsSimulationUIOpen = false;

    WindSpeed = DefaultWindSpeed = glm::vec2(1.0f, 2.0f);
    IsWindEnabled = false;
    
    Gravity = DefaultGravity = glm::vec3(0, 0.98f, 0);
    
    ClothResolution = DefaultClothSize = glm::vec2(30, 30);
    Mass = DefaultMass = 2.0f;
    Stiffness = DefaultStiffness = 1.0f;
    Damping = DefaultDamping = 0.01f;
    NumberOfConstraintIterations = DefaultNumberOfConstraintIterations = 15;
    ClothColor = DefaultClothColor = glm::vec3(1.0f, 1.0f, 1.0f);
    SegmentLength = DefaultSegmentLength = CalculateSegmentLength(ClothResolution);

    ballRadius = 0.25f;
    ballPosition = glm::vec3(SegmentLength * ClothResolution.y * 0.5f, SegmentLength * ClothResolution.x * 1.8f, ballRadius * 2);

    integrationMethodType = IntegratorType::Verlet;

    VerletDrag = DefaultVerletDrag = 0.01f;

    ExplicitEulerCollisionOffset = DefaultExplicitEulerTimeStep = 0.15f;
    VerletCollisionOffset = DefaultVerletCollisionOffset = 0.07f;
    SemiImplicitCollisionOffset = DefaultSemiImplicitCollisionOffset = 0.165f;
    Rk4tCollisionOffset = DefaultRk4tCollisionOffset = 0.165f;

    ExplicitEulerTimeStep = DefaultExplicitEulerTimeStep = 0.015f;
    VerletTimeStep = DefaultVerletTimeStep = 0.0015f;
    SemiImplicitTimeStep = DefaultSemiImplicitTimeStep = (float)1 / 60.0f;
    Rk4TimeStep = DefaultRk4TimeStep = (float)1 / 60.0f;

    Initialize();
}

void Scene::Initialize()
{

    light = std::make_unique<Light>();
    light->Color = LightColor;
    light->Position = LightPosition;

    explicitEulerInegrator = std::make_unique<ExplicitEulerIntegrator>();
    verletInegrator = std::make_unique<VerletIntegrator>(VerletDrag);
    semiEulerInegrator = std::make_unique<SemiImplicitEulerIntegrator>();
    rk4Integrator = std::make_unique<RK4Integrator>();

    gravitationalForce = std::make_unique<GravitationalForce>(Gravity);
    springForce = std::make_unique<SpringForce>(ClothResolution.x, ClothResolution.y, SegmentLength, Stiffness, Damping);

    windForce = std::make_unique<WindForce>(WindSpeed.x / 1000, WindSpeed.y / 1000);
    windForce->IsEnabled = false;
}

void Scene::Update(bool keyState[], Camera& camera, float deltaTime)
{
    if (keyState[GLFW_KEY_F1] && !wasF1Pressed)
    {
        IsSimulationUIOpen = !IsSimulationUIOpen;
    }

    wasF1Pressed = keyState[GLFW_KEY_F1];

    light->Color = LightColor;
    light->Position = LightPosition;
    cloth->Mass = Mass;
    cloth->Color = ClothColor;
    cloth->NumberOfConstraintIterations = NumberOfConstraintIterations;
    
    gravitationalForce->Gravity = Gravity;

    windForce->IsEnabled = IsWindEnabled;
    windForce->MinimumSpeed = WindSpeed.x / 1000;
    windForce->MaximumSpeed = WindSpeed.y / 1000;
    
    springForce->Stiffness = Stiffness;
    springForce->Damping = Damping;

    switch (integrationMethodType)
    {
        case IntegratorType::ExplicitEuler:
        {
            CurrentTimeStep = ExplicitEulerTimeStep;
            CurrentCollisionOffset = ExplicitEulerCollisionOffset;
            cloth->IntergrationMethod = explicitEulerInegrator.get();
            break;
        }
        case IntegratorType::Verlet:
        {
            CurrentTimeStep = VerletTimeStep;
            CurrentCollisionOffset = VerletCollisionOffset;
            cloth->IntergrationMethod = verletInegrator.get();
            break;
        }
        case IntegratorType::SemiImplicitEuler:
        {
            CurrentTimeStep = SemiImplicitTimeStep;
            CurrentCollisionOffset = SemiImplicitCollisionOffset;
            cloth->IntergrationMethod = semiEulerInegrator.get();
            break;
        }
        case IntegratorType::RK4:
        {
            CurrentTimeStep = Rk4TimeStep;
            CurrentCollisionOffset = Rk4tCollisionOffset;
            cloth->IntergrationMethod = rk4Integrator.get();
            break;
        }
    }        
}

void Scene::ResetClothDefaults()
{
    ClothResolution = DefaultClothSize;
    Mass = DefaultMass;
    Stiffness = DefaultStiffness;
    Damping = DefaultDamping;
    NumberOfConstraintIterations = DefaultNumberOfConstraintIterations;
    ClothColor = DefaultClothColor;
    SegmentLength = DefaultSegmentLength;
}

void Scene::ResetAllDefaults()
{
    RecreateCloth();
    ResetClothDefaults();
    LightColor = DefaultLightColor;
    LightPosition = DefaultLightPosition;

    DrawInWireFrame = false;
    WindSpeed = DefaultWindSpeed;
    IsWindEnabled = false;

    Gravity = DefaultGravity;

    integrationMethodType = IntegratorType::Verlet;

    VerletDrag = DefaultVerletDrag;

    ExplicitEulerCollisionOffset = DefaultExplicitEulerTimeStep;
    VerletCollisionOffset = DefaultVerletCollisionOffset;
    SemiImplicitCollisionOffset = DefaultSemiImplicitCollisionOffset;
    Rk4tCollisionOffset = DefaultRk4tCollisionOffset;

    ExplicitEulerTimeStep = DefaultExplicitEulerTimeStep;
    VerletTimeStep = DefaultVerletTimeStep;
    SemiImplicitTimeStep = DefaultSemiImplicitTimeStep;
    Rk4TimeStep = DefaultRk4TimeStep;
}

float Scene::CalculateSegmentLength(glm::vec2 resolution)
{
    return 1.0f / (float)(std::max(resolution.y - 1, resolution.x - 1));
}

void Scene::DrawUI(float deltaTime)
{

    if (!IsSimulationUIOpen)
        return;

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Scene Properties"))
    {
        ImGui::InputFloat3("Light Position", &LightPosition[0]);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));
        ImGui::Checkbox("WireFrame", &DrawInWireFrame);
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Simulation Properties"))
    {
        ImGui::SliderFloat3("Gravity", glm::value_ptr(Gravity), 0, 10, "%.7f", 0);
        ImGui::Checkbox("Enable Wind", &IsWindEnabled);
        ImGui::SliderFloat2("Wind Speed", glm::value_ptr(WindSpeed), 0, 4, "%.7f", 0);
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Cloth Properties"))
    {
        ImGui::InputFloat2("Resolution", glm::value_ptr(ClothResolution));
        ImGui::ColorEdit3("Color", glm::value_ptr(ClothColor));
        ImGui::SliderInt("Constraint Iterations", &NumberOfConstraintIterations, 0, 50);
        ImGui::SliderFloat("Stiffness", &Stiffness, 0, 10, "%.7f", 0);
        ImGui::SliderFloat("Damping", &Damping, 0, 1, "%.7f", 0);
        ImGui::SliderFloat("Mass", &Mass, 2, 50, "%.7f", 0);
        if (ImGui::Button("Recreate"))
        {
            RecreateCloth();
        }
        if (ImGui::Button("Reset to Defaults"))
        {
            ResetClothDefaults();
            RecreateCloth();
        }
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Intergration Methods"))
    {
        ImGui::Combo("Inegration Methods", &integrationMethodType, intergrationMethodNames, IM_ARRAYSIZE(intergrationMethodNames));

        switch (integrationMethodType)
        {
            case IntegratorType::Verlet:
            {
                ImGui::SliderFloat("Drag", &VerletDrag, 0, 1, "%.7f", 0);
                ImGui::SliderFloat("Time Step", &VerletTimeStep, 0, 2, "%.7f", 0);
                ImGui::SliderFloat("Collision Offset", &VerletCollisionOffset, 0, 2, "%.7f", 0);
                break;
            }
            case IntegratorType::ExplicitEuler:
            {
                ImGui::SliderFloat("Time Step", &ExplicitEulerTimeStep, 0, 0.5, "%.7f", 0);
                ImGui::SliderFloat("Collision Offset", &ExplicitEulerCollisionOffset, 0, 0.9, "%.7f", 0);
                break;
            }
            case IntegratorType::SemiImplicitEuler:
            {
                ImGui::SliderFloat("Time Step", &SemiImplicitTimeStep, 0, 0.5, "%.7f", 0);
                ImGui::SliderFloat("Collision Offset", &SemiImplicitCollisionOffset, 0, 0.9, "%.7f", 0);
                break;
            }
            case IntegratorType::RK4:
            {
                ImGui::SliderFloat("Time Step", &Rk4TimeStep, 0, 0.5, "%.7f", 0);
                ImGui::SliderFloat("Collision Offset", &Rk4tCollisionOffset, 0, 0.9, "%.7f", 0);
                break;
            }
        }


        if (ImGui::Button("Restart"))
        {
            RecreateCloth();
        }
        ImGui::TreePop();
    }

    if (ImGui::Button("Reset All Defaults"))
    {
        ResetAllDefaults();
        RecreateCloth();
    }
}

void Scene::BeginUI()
{
    if (!IsSimulationUIOpen)
        return;

    ImGui::Begin(windowTitle.c_str(), &IsSimulationUIOpen);
    ImGui::SetWindowPos(windowTitle.c_str(), ImVec2(applicationWindowWidth - 650, 50), ImGuiCond_Once);
}

void Scene::EndUI()
{
    if (!IsSimulationUIOpen)
        return;
    ImGui::End();
}
