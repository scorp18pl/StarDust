#pragma once

#include "Test.h"

#include <glm/mat4x4.hpp>

class PrimitiveTest : public Test
{
public:
    PrimitiveTest();

    void OnUpdate(float deltaTime) override;
    void OnRender(Str::Window& window) override;
    void OnImGuiRender() override;

private:
    static constexpr float Divisor = 32.0f;

    bool m_rotationEnabled = false;

    const float m_speedMultiplier = 0.00001f;
    float m_rotSpeed = 1.0f;

    Uni::Math::Vector3f m_translation = Uni::Math::Vector3f::CreateZero();
    Uni::Math::Vector3f m_rotation = Uni::Math::Vector3f::CreateZero();
    Uni::Grpx::Color m_color = Uni::Grpx::Color::CreateFromVector3f(
        Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f);
    float m_scale = 1.0f;

    std::string m_currentName = "Triangle";
    Str::PrimitiveType m_currentPrimitive = Str::PrimitiveType::Triangle;

    Str::ModelInstance m_instance = Str::ModelInstance(
        m_currentPrimitive,
        Uni::Math::Transform(),
        Uni::Grpx::Color::CreateFromVector3f(
            Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f));

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};
