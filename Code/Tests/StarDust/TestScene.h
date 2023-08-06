#pragma once

#include "Test.h"
#include <StarDust/LightSource.h>
#include <vector>

class TestScene : public Test
{
public:
    TestScene(TestType testType = TestType::Scene);
    ~TestScene() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

protected:
    std::vector<Star::ModelInstance> m_instances;
    float m_deltaTime = 1.0f;
    unsigned int m_currentInstance = 0U;

    std::vector<std::pair<int, Star::LightSourceType>> m_lights;
    unsigned int m_currentLight = 0U;

    Uni::Math::Matrix4x4f m_viewMatrix;

    Uni::Math::Vector3f m_cameraTranslation;
    float m_cameraPitch = 0.0f;
    float m_cameraYaw = 0.0f;

    bool m_cameraMode = false;

    bool m_Perspective = true;
    float m_fieldOfView = 45.0f;
    float m_aspectRatio = 1.0f;
    Uni::Math::Vector3f m_orthoSize = Uni::Math::Vector3f{ 100.0f };

    Uni::Math::Matrix4x4f m_projectionMatrix;

    void HandleCamera(Star::Window& window);
};
