#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "PrimitiveTest.h"

PrimitiveTest::PrimitiveTest()
    : Test(Test::TestType::Primitive)
{
    m_instance = Str::ModelInstance(
        m_currentPrimitive,
        Uni::Math::Transform(),
        Uni::Grpx::Color::CreateFromVector3f(
            Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f));

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

void PrimitiveTest::OnUpdate(float deltaTime)
{
    m_instance.GetTransform().SetTranslation(m_translation);
    if (m_rotationEnabled)
    {
        m_instance.GetTransform().Rotate(
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                m_rotSpeed * m_speedMultiplier * deltaTime,
                Uni::Math::Axis::Z));
    }
    else
    {
        m_instance.GetTransform().SetRotation(
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                m_rotation.m_x, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                m_rotation.m_y, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                m_rotation.m_z, Uni::Math::Axis::Z));
    }

    m_instance.GetTransform().SetScale(
        Uni::Math::Vector3f::CreateFromFloat(m_scale));
    m_instance.GetColor() = m_color;

    m_instance.Update();
}

void PrimitiveTest::OnRender(Str::Window& window)
{
    m_projectionMatrix = glm::perspective(
        glm::radians(90.0f),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        100.0f);

    Str::ShaderProgram& shader =
        Str::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
    shader.Bind();

    shader.SetUniformMat4f("u_view", m_viewMatrix);
    shader.SetUniformMat4f("u_proj", m_projectionMatrix);
}

void PrimitiveTest::OnImGuiRender()
{
    static const std::array<std::pair<std::string, Str::PrimitiveType>, 4>
        Primitives = {
            std::make_pair("Triangle", Str::PrimitiveType::Triangle),
            std::make_pair("Rectangle", Str::PrimitiveType::Rectangle),
            std::make_pair("Box", Str::PrimitiveType::Box),
            std::make_pair("Icosahedron", Str::PrimitiveType::Icosahedron),
        };

    if (ImGui::BeginCombo(
            "PrimitiveType", m_currentName.c_str(), ImGuiComboFlags_None))
    {
        for (const auto& primitive : Primitives)
        {
            bool isSelected = (m_currentName == primitive.first);
            if (ImGui::Selectable(
                    primitive.first.c_str(),
                    isSelected,
                    ImGuiSelectableFlags_None))
            {
                m_currentName = primitive.first;
                m_currentPrimitive = primitive.second;
                m_instance = Str::ModelInstance(
                    m_currentPrimitive,
                    Uni::Math::Transform(),
                    Uni::Grpx::Color::CreateFromVector3f(
                        Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f));
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
