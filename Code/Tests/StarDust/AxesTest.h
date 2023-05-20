#pragma once

#include "Test.h"
#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Model/MeshRegistry.h>
#include <Universal/Math/Transform.h>
#include <glm/glm.hpp>

class AxesTest : public Test
{
public:
    AxesTest();
    ~AxesTest() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    Star::ModelInstance m_axisX{ Star::MeshRegistry::Get().GetMeshId("Box") };
    Star::ModelInstance m_axisY{ Star::MeshRegistry::Get().GetMeshId("Box") };
    Star::ModelInstance m_axisZ{ Star::MeshRegistry::Get().GetMeshId("Box") };

    Uni::Math::Transform m_rootTransform;

    Star::LightSource m_lightSource;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};
