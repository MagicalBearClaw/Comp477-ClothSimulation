#include "../stdafx.h"
#include "Scene.h"

Scene::Scene(const std::string& windowTitle , int applicationWindowWidth, int applicationWindowHeight) 
            : windowTitle(windowTitle), applicationWindowWidth(applicationWindowWidth), applicationWindowHeight(applicationWindowHeight)
{
    LightColor = DefaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    LightPosition = DefaultLightPosition = glm::vec3(0.2f, 1.8f, 1.0f);

    DrawInWireFrame = false;
    IsSimulationUIOpen = false;

    WindSpeed = DefaultWindSpeed = glm::vec2(0.0005f, 0.002f);
    IsWindEnabled = false;
    
    Gravity = DefaultGravity = glm::vec3(0, 0.98f, 0);
    
    ClothSize = DefaultClothSize = glm::vec2(30, 30);
    Mass = DefaultMass = 1.0f;
    Stiffness = DefaultStiffness = 2.0f;
    NumberOfConstraintIterations = DefaultNumberOfConstraintIterations = 15;
    ClothColor = DefaultClothColor = glm::vec3(1.0f, 1.0f, 1.0f);
    SegmentLength = DefaultSegmentLength = 1.0f / (float)(std::max(ClothSize.y - 1, ClothSize.x - 1));

    ballRadius = 0.25f;
    ballPosition = glm::vec3(SegmentLength * ClothSize.y * 0.5f, SegmentLength * ClothSize.x * 1.8f, ballRadius * 2);

    integrationMethodType = IntegratorType::Verlet;

    VerletDrag = DefaultVerletDrag = 0.01f;

    ExplicitEulerCollisionOffset = DefaultExplicitEulerTimeStep = 0.17f;
    VerletCollisionOffset = DefaultVerletCollisionOffset = 0.07f;
    SemiImplicitCollisionOffset = DefaultSemiImplicitCollisionOffset = 0.17f;
    Rk4tCollisionOffset = DefaultRk4tCollisionOffset = 0.17f;

    ExplicitEulerTimeStep = DefaultExplicitEulerTimeStep = 0.00015f;
    VerletTimeStep = DefaultVerletTimeStep = (float)1 / 60.0f;
    SemiImplicitTimeStep = DefaultSemiImplicitTimeStep = (float)1 / 60.0f;
    Rk4TimeStep = DefaultRk4TimeStep = (float)1 / 60.0f;

    Initialize();
}

void Scene::Initialize()
{

    light = std::make_unique<Light>();
    light->Color = LightColor;
    light->Position = LightPosition;

    std::string catTexture = std::filesystem::path("./Assets/Textures/cat2.jpg").generic_u8string();
    cloth = std::make_unique<Cloth>(ClothSize.x, ClothSize.y, catTexture);

    explicitEulerInegrator = std::make_unique<ExplicitEulerIntegrator>();
    verletInegrator = std::make_unique<VerletIntegrator>(VerletDrag);
    semiEulerInegrator = std::make_unique<SemiImplicitEulerIntegrator>();
    rk4Integrator = std::make_unique<RK4Integrator>();

    gravitationalForce = std::make_unique<GravitationalForce>(Gravity);
    springForce = std::make_unique<SpringForce>(ClothSize.x, ClothSize.y, SegmentLength, Stiffness);

    windForce = std::make_unique<WindForce>(WindSpeed.x, WindSpeed.y);
    windForce->IsEnabled = false;
    
    cloth->IntergrationMethod = verletInegrator.get();

    cloth->AddForceGenerator(gravitationalForce.get());
    cloth->AddForceGenerator(springForce.get());
    cloth->AddForceGenerator(windForce.get());

}

void Scene::Update(bool keyState[], float deltaTime)
{
    if (keyState[GLFW_KEY_F1])
    {
        IsSimulationUIOpen = !IsSimulationUIOpen;
    }

    light->Color = LightColor;
    light->Position = LightPosition;

    switch (integrationMethodType)
    {
        case IntegratorType::ExplicitEuler:
        {
            CurrentTimeStep = ExplicitEulerTimeStep;
            CurrentCollisionOffset = ExplicitEulerCollisionOffset;
            break;
        }
        case IntegratorType::Verlet:
        {
            CurrentTimeStep = VerletTimeStep;
            CurrentCollisionOffset = VerletCollisionOffset;
            break;
        }
        case IntegratorType::SemiImplicitEuler:
        {
            CurrentTimeStep = SemiImplicitTimeStep;
            CurrentCollisionOffset = SemiImplicitCollisionOffset;
            break;
        }
        case IntegratorType::RK4:
        {
            CurrentTimeStep = Rk4TimeStep;
            CurrentCollisionOffset = Rk4tCollisionOffset;
            break;
        }
    }

}

void Scene::DrawUI(float deltaTime)
{
    if (!IsSimulationUIOpen)
        return;

    ImGui::Begin(windowTitle.c_str(), &IsSimulationUIOpen);
    ImGui::SetWindowPos(windowTitle.c_str(), ImVec2(applicationWindowWidth - 420, 50), ImGuiCond_Once);

    if (deltaTimes.size() >= maxDeltaTimes)
    {
        deltaTimes.pop_front();
    }
    deltaTimes.push_back(deltaTime * 1000);

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Simulation statistics"))
    {
        ImGui::PlotLines("Frame Times", &deltaTimes[0], deltaTimes.size(), 0, 0, 100, 0, ImVec2(250, 50));
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Scene Properties"))
    {
        ImGui::InputFloat2("Light Position", &LightPosition[0]);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));
        ImGui::Checkbox("WireFrame", &DrawInWireFrame);
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Simulation Properties"))
    {
        ImGui::InputFloat3("Gravity", glm::value_ptr(Gravity));
        ImGui::Checkbox("Enable Wind", &IsWindEnabled);
        ImGui::InputFloat2("Wind Speed", glm::value_ptr(WindSpeed));
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Cloth Properties"))
    {
        ImGui::InputFloat2("Size", glm::value_ptr(ClothSize));
        ImGui::InputFloat("Segment Length", &SegmentLength);
        ImGui::ColorEdit3("Color", glm::value_ptr(ClothColor));
        ImGui::InputInt("Constraint Iterations", &NumberOfConstraintIterations);
        ImGui::InputFloat("Stiffness", &Stiffness);
        ImGui::InputFloat("Mass", &Mass);
        if (ImGui::Button("ReCreate"))
        {
            // todo: handle recreation of the cloth
        }
        if (ImGui::Button("Reset"))
        {
            // todo: handle recreation of the cloth
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
                ImGui::InputFloat("Drag", &VerletDrag);
                ImGui::InputFloat("Time Step", &VerletTimeStep);
                ImGui::InputFloat("Collision Offset", &VerletCollisionOffset);
                break;
            }
            case IntegratorType::ExplicitEuler:
            {
                ImGui::InputFloat("Time Step", &ExplicitEulerTimeStep);
                ImGui::InputFloat("Collision Offset", &ExplicitEulerCollisionOffset);
                break;
            }
            case IntegratorType::SemiImplicitEuler:
            {
                ImGui::InputFloat("Time Step", &SemiImplicitTimeStep);
                ImGui::InputFloat("Collision Offset", &SemiImplicitCollisionOffset);
                break;
            }
            case IntegratorType::RK4:
            {
                ImGui::InputFloat("Time Step", &SemiImplicitTimeStep);
                ImGui::InputFloat("Collision Offset", &SemiImplicitCollisionOffset);
                break;
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::Button("Reset Defaults"))
    {
        // todo: handle recreation of the cloth and re-apply defaults;
    }

    ImGui::End();
}

void Scene::Reset()
{
}
