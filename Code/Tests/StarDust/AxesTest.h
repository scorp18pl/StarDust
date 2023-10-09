#pragma once

#include "Test.h"
#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Model/ModelInstance3D.h>
#include <Universal/Math/Transform.h>

class AxesTest : public Test
{
public:
    AxesTest();
    ~AxesTest() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    Star::ModelInstance3D m_axisX{ Star::Mesh::GetId("Box") };
    Star::ModelInstance3D m_axisY{ Star::Mesh::GetId("Box") };
    Star::ModelInstance3D m_axisZ{ Star::Mesh::GetId("Box") };

    Uni::Math::Transform m_rootTransform;

    Star::LightSource m_lightSource;
    Uni::Math::Matrix4x4f m_viewMatrix;
    Uni::Math::Matrix4x4f m_projectionMatrix;
};
