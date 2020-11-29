#include "../stdafx.h"
#include "Scene.h"

Scene::Scene(const std::string& windowTitle , int applicationWindowWidth, int applicationWindowHeight) 
            : windowTitle(windowTitle), applicationWindowWidth(applicationWindowWidth), applicationWindowHeight(applicationWindowHeight)
{
}

void Scene::Initialize()
{

}

void Scene::DrawUI(float deltaTime)
{
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
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Simulation Properties"))
    {
        ImGui::InputFloat3("Gravity", glm::value_ptr(Gravity));
        ImGui::Checkbox("Enable Wind", &IsWindEnabled);
        ImGui::InputFloat3("Wind Speed", glm::value_ptr(WindSpeed));
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Cloth Properties"))
    {
        ImGui::InputFloat2("Size", glm::value_ptr(ClothSize));
        ImGui::ColorEdit3("Color", glm::value_ptr(ClothColor));
        ImGui::InputInt("Constraint Iterations", &NumberOfConstraintIterations);
        ImGui::InputFloat("Stiffness", &Stiffness);
        ImGui::InputFloat("Mass", &Mass);
        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("Intergration Methods"))
    {
        ImGui::Combo("Inegration Methods", &integrationMethodType, intergrationMethodNames, IM_ARRAYSIZE(intergrationMethodNames));
        ImGui::InputFloat("Time Step", &TimeStep);
        ImGui::InputFloat("Collision Offset", &CollisionOffset);
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

    ImGui::End();
}

void Scene::Reset()
{
}
