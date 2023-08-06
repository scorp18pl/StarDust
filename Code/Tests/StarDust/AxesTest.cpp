#include "AxesTest.h"

#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utilities/Math.h>
#include <Universal/Math/Math.h>
#include <imgui.h>

AxesTest::AxesTest()
    : Test(TestType::Axes)
    , m_lightSource(
          Star::LightSourceType::Directional,
          Star::LightData{
              Uni::Math::Vector3f{ 0.0f, 0.0f, -1.0f },
              Uni::Grpx::Color::White,
              1.0f,
          })
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
    m_axisX.Update();
    m_axisY.Update();
    m_axisZ.Update();
}

void AxesTest::OnRender(Star::Window& window)
{
    m_projectionMatrix = Star::Utils::CreateOrthographicProjectionMatrix(
        -static_cast<float>(window.GetWidth()) / 32.0f,
        static_cast<float>(window.GetWidth()) / 32.0f,
        -static_cast<float>(window.GetHeight()) / 32.0f,
        static_cast<float>(window.GetHeight()) / 32.0f,
        0.1f,
        10000.0f);

    m_viewMatrix = Star::Utils::CreateLookAtMatrix(
        Uni::Math::Vector3f{ 50.0f },
        Uni::Math::Vector3f::CreateZero(),
        Uni::Math::Vector3f::CreateAxisZ());

    Star::ShaderProgram& shader =
        Star::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
    shader.Bind();
    shader.SetUniformMat4x4f("u_view", m_viewMatrix);
    shader.SetUniformMat4x4f("u_proj", m_projectionMatrix);
}

void AxesTest::OnImGuiRender()
{
    Uni::Math::Vector3f rotationDegrees =
        m_rootTransform.GetRotation().GetEulerRadZYX() *
        Uni::Math::Constants::RadToDeg;
    Uni::Math::Vector3f translation = m_rootTransform.GetTranslation();

    ImGui::Text("Coordinate system (X - Red, Y - Green, Z - Blue)");
    ImGui::SliderFloat3("Rotation (Deg.):", rotationDegrees.m_data, 0.0f, 90.0f);
    ImGui::SliderFloat3(
        "Translation (Meters):", translation.m_data, -20.0f, 20.0f);

    m_rootTransform.SetRotation(Uni::Math::Quaternion::CreateFromEulerRadZYX(
        rotationDegrees * Uni::Math::Constants::DegToRad));
    m_rootTransform.SetTranslation(translation);
}
