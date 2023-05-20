#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "MeshTest.h"

MeshTest::MeshTest()
    : Test(Test::TestType::Mesh)
    , m_lightSource(
          Star::LightSourceType::Directional,
          Star::LightData{
              Uni::Math::Vector3f{ 0.0f, 0.0f, -1.0f },
              Uni::Grpx::Color::White,
              2.0f,
          })
{
    m_instance = Star::ModelInstance(
        m_currentMeshId,
        Uni::Math::Transform(),
        Uni::Grpx::Color::CreateFromVector3f(
            Uni::Math::Vector3f{ 1.0f }, 1.0f));

    m_viewMatrix = glm::lookAt(
        glm::vec3(
            50.0f,
            0.0f,
            0.0f), // Camera is at in World Space
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void MeshTest::OnUpdate(float deltaTime)
{
    m_instance.GetTransform().SetTranslation(m_translation);
    if (m_rotationEnabled)
    {
        m_instance.GetTransform().Rotate(
            Uni::Math::Quaternion::CreateFromAxisRad(
                m_rotSpeed * m_speedMultiplier * deltaTime,
                Uni::Math::Vector3f::CreateAxisZ()));
    }
    else
    {
        m_instance.GetTransform().SetRotation(
            Uni::Math::Quaternion::CreateFromEulerDegZYX(m_rotation));
    }

    m_instance.GetTransform().SetScale(Uni::Math::Vector3f{ m_scale });
    m_instance.GetColor() = m_color;

    m_instance.Update();
}

void MeshTest::OnRender(Star::Window& window)
{
    m_projectionMatrix = glm::perspective(
        glm::radians(90.0f),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        100.0f);

    Star::ShaderProgram& shader =
        Star::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
    shader.Bind();

    shader.SetUniformMat4f("u_view", m_viewMatrix);
    shader.SetUniformMat4f("u_proj", m_projectionMatrix);
}

void MeshTest::OnImGuiRender()
{
    if (ImGui::BeginCombo(
            "PrimitiveType", m_currentName.c_str(), ImGuiComboFlags_None))
    {
        for (const auto& [meshId, mesh] :
             Star::MeshRegistry::Get().GetRegisteredMeshes())
        {
            bool isSelected = (m_currentName == mesh.m_name);
            if (ImGui::Selectable(
                    mesh.m_name.c_str(), isSelected, ImGuiSelectableFlags_None))
            {
                m_currentName = mesh.m_name;
                m_currentMeshId = meshId;
                m_instance = Star::ModelInstance(
                    m_currentMeshId,
                    Uni::Math::Transform(),
                    Uni::Grpx::Color::CreateFromVector3f(
                        Uni::Math::Vector3f{ 1.0f }, 1.0f));
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SliderFloat3(
        "Translation (Meters)", &m_translation.m_x, -20.0f, 20.0f);
    if (ImGui::Button("Reset Translation"))
    {
        m_translation = Uni::Math::Vector3f::CreateZero();
    }

    ImGui::SliderFloat3("Rotation (Deg)", &m_rotation.m_x, -180.0f, 180.0f);
    if (ImGui::Button("Reset Rotation"))
    {
        m_rotation = Uni::Math::Vector3f::CreateZero();
    }

    if (ImGui::Button(
            m_rotationEnabled ? "Disable Rotation" : "Enable Rotation"))
    {
        m_rotationEnabled = !m_rotationEnabled;
    }

    if (m_rotationEnabled)
    {
        ImGui::SliderFloat("Rotation Speed", &m_rotSpeed, 0.0f, 360.0f);
    }

    ImGui::SliderFloat("Scale", &m_scale, 1.0f, 30.0f);

    ImGui::ColorEdit4(
        "Color", static_cast<float*>(static_cast<void*>(&m_color)));
}
