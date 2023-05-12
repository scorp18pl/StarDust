#include "AxesTest.h"

#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

AxesTest::AxesTest()
    : Test(TestType::Axes)
{
    m_axisX.GetColor() = Uni::Grpx::Color::Red;
    m_axisY.GetColor() = Uni::Grpx::Color::Green;
    m_axisZ.GetColor() = Uni::Grpx::Color::Blue;

    m_axisX.GetTransform().SetTranslation({
        10.0f,
        0.0f,
        0.0f,
    });
    m_axisX.GetTransform().SetScale({
        20.0f,
        1.0f,
        1.0f,
    });

    m_axisY.GetTransform().SetTranslation({
        0.0f,
        10.0f,
        0.0f,
    });
    m_axisY.GetTransform().SetScale({
        1.0f,
        20.0f,
        1.0f,
    });

    m_axisZ.GetTransform().SetTranslation({
        0.0f,
        0.0f,
        10.0f,
    });
    m_axisZ.GetTransform().SetScale({
        1.0f,
        1.0f,
        20.0f,
    });

    m_axisX.GetTransform().SetParent(&m_rootTransform);
    m_axisY.GetTransform().SetParent(&m_rootTransform);
    m_axisZ.GetTransform().SetParent(&m_rootTransform);
}

void AxesTest::OnUpdate(float deltaTime)
{
    m_rootTransform.SetTranslation(m_translation);
    m_rootTransform.SetRotation(
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            m_rotation.m_x, Uni::Math::Axis::X) *
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            m_rotation.m_y, Uni::Math::Axis::Y) *
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            m_rotation.m_z, Uni::Math::Axis::Z));

    m_axisX.Update();
    m_axisY.Update();
    m_axisZ.Update();
}

void AxesTest::OnRender(Str::Window& window)
{
    m_projectionMatrix = glm::ortho(
        -static_cast<float>(window.GetWidth()) / 32.0f,
        static_cast<float>(window.GetWidth()) / 32.0f,
        -static_cast<float>(window.GetHeight()) / 32.0f,
        static_cast<float>(window.GetHeight()) / 32.0f,
        0.1f,
        10000.0f);
    m_viewMatrix = glm::lookAt(
        glm::vec3(
            80.0f,
            80.0f,
            80.0f), // Camera is at in World Space
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );

    Str::ShaderProgram& shader =
        Str::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_viewMatrix);
    shader.SetUniformMat4f("u_proj", m_projectionMatrix);
}

void AxesTest::OnImGuiRender()
{
    ImGui::Text("Coordinate system (X - Red, Y - Green, Z - Blue)");
    ImGui::SliderFloat3(
        "Rotation (Rad):",
        static_cast<float*>(static_cast<void*>(&m_rotation)),
        0.0f,
        2.0f);
    ImGui::SliderFloat3(
        "Translation (Meters):",
        reinterpret_cast<float*>(static_cast<void*>(&m_translation)),
        -20.0f,
        20.0f);
}
