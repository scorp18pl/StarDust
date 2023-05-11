#include "InstancingTest.h"

#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <Universal/Math/Random/Generator.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

InstancingTest::InstancingTest()
    : Test(TestType::Instancing)
{
    Uni::Math::Rand::Generator generator;
    generator.SetRandomSeed();

    m_instances.reserve(InstanceCount);
    for (size_t i = 0; i < InstanceCount; ++i)
    {
        const Uni::Math::Vector3f dim = Uni::Math::Vector3f::CreateFromFloat(
            generator.GenerateInRange(25.0f, 75.0f));

        Uni::Math::Transform transform;
        transform.SetScale(dim);
        transform.SetTranslation({
            generator.GenerateInRange(MinBound.m_x, MaxBound.m_x),
            generator.GenerateInRange(MinBound.m_y, MaxBound.m_y),
            generator.GenerateInRange(MinBound.m_z, MaxBound.m_z),
        });

        m_instances.emplace_back(
            Str::PrimitiveType::Icosahedron,
            transform,
            Uni::Grpx::Color::CreateFromVector3f(
                Uni::Math::Vector3f::CreateRandomUnitVector(generator) * 2.0f,
                1.0f));

        m_velocities.emplace_back(
            Uni::Math::Vector3f::CreateRandomUnitVector(generator) * 0.01f);
    }

    m_viewMatrix = glm::lookAt(
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // Camera is at in World Space
        glm::vec3(
            1.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void InstancingTest::OnUpdate(float deltaTime)
{
    for (auto& instance : m_instances)
    {
        Uni::Math::Matrix3x4f rotation =
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                m_rotSpeed * 0.001f * deltaTime, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                m_rotSpeed * 0.002f * deltaTime, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                m_rotSpeed * 0.003f * deltaTime, Uni::Math::Axis::Z);

        const Uni::Math::Vector3f currentTranslation =
            instance.GetTransform().GetTranslation();

        if (currentTranslation.m_x > MaxBound.m_x ||
            currentTranslation.m_x < MinBound.m_x)
        {
            m_velocities[&instance - &m_instances[0]].m_x *= -1.0f;
        }

        if (currentTranslation.m_y > MaxBound.m_y ||
            currentTranslation.m_y < MinBound.m_y)
        {
            m_velocities[&instance - &m_instances[0]].m_y *= -1.0f;
        }

        if (currentTranslation.m_z > MaxBound.m_z ||
            currentTranslation.m_z < MinBound.m_z)
        {
            m_velocities[&instance - &m_instances[0]].m_z *= -1.0f;
        }

        instance.GetTransform().Rotate(rotation);
        instance.GetTransform().Translate(
            { m_velocities[&instance - &m_instances[0]] * m_speedMultiplier *
              deltaTime });
    }

    m_viewMatrix = glm::rotate(
        m_viewMatrix,
        m_rotSpeed * 0.001f * deltaTime,
        glm::vec3(1.0f, 2.0f, 3.0f));
}

void InstancingTest::OnRender(Str::Window& window)
{
    m_projectionMatrix = glm::perspective(
        glm::radians(m_fieldOfView),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        10000.0f);

    Str::ShaderProgram& shader =
        Str::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_viewMatrix);
    shader.SetUniformMat4f("u_proj", m_projectionMatrix);
    shader.SetUniform3f("u_viewPosition", 0.0f, 0.0f, 0.0f);

    for (auto& instance : m_instances)
    {
        window.Draw(instance);
    }
}

void InstancingTest::OnImGuiRender()
{
    ImGui::Text("Instances");
    ImGui::SliderFloat("Rotation Speed", &m_rotSpeed, 0.00001f, 1.0f);
    ImGui::SliderFloat("Translation Speed", &m_speedMultiplier, 1.0f, 100.0f);
    ImGui::SliderFloat("FOV", &m_fieldOfView, 0.0f, 160.0f);
}
