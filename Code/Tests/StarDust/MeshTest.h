#pragma once

#include "Test.h"
#include <StarDust/Model/Mesh.h>

class MeshTest : public Test
{
public:
    MeshTest();

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    static constexpr float Divisor = 32.0f;

    bool m_rotationEnabled = false;

    const float m_speedMultiplier = 0.00001f;
    float m_rotSpeed = 1.0f;

    Uni::Math::Vector3f m_translation = Uni::Math::Vector3f::CreateZero();
    Uni::Math::Vector3f m_rotation = Uni::Math::Vector3f::CreateZero();
    Uni::Grpx::Color m_color =
        Uni::Grpx::Color::CreateFromVector3f(Uni::Math::Vector3f{ 1.0f }, 1.0f);
    float m_scale = 1.0f;
    Star::LightSource m_lightSource;

    std::string m_currentName = "Triangle";
    Star::Mesh::IdType m_currentMeshId =
        Star::Mesh::GetId(m_currentName);

    Star::ModelInstance3D m_instance = Star::ModelInstance3D(
        m_currentMeshId,
        Uni::Math::Transform(),
        Uni::Grpx::Color::CreateFromVector3f(
            Uni::Math::Vector3f{ 1.0f }, 1.0f));

    Uni::Math::Matrix4x4f m_viewMatrix;
    Uni::Math::Matrix4x4f m_projectionMatrix;
};
