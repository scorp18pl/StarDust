#pragma once

#include "Test.h"
#include <glm/glm.hpp>

class AxesTest : public Test
{
public:
    AxesTest();
    ~AxesTest() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Str::Window& window) override;
    void OnImGuiRender() override;

private:
    Str::ModelInstance m_axisX{ Str::PrimitiveType::Box };
    Str::ModelInstance m_axisY{ Str::PrimitiveType::Box };
    Str::ModelInstance m_axisZ{ Str::PrimitiveType::Box };

    Uni::Math::Transform m_rootTransform;

    Uni::Math::Vector3f m_rotation{ 0.0f, 0.0f, 0.0f };
    Uni::Math::Vector3f m_translation{ 0.0f, 0.0f, 0.0f };

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};
