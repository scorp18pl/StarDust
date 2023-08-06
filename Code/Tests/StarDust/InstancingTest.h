#pragma once

#include "Test.h"

#include <Universal/Math/Matrix/Matrix4x4f.h>

class InstancingTest : public Test
{
public:
    InstancingTest();
    ~InstancingTest() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;
private:
    static constexpr unsigned int InstanceCount = 100000U;
    const Uni::Math::Vector3f MaxBound = { 10000.0f, 10000.0f, 10000.0f };
    const Uni::Math::Vector3f MinBound = -MaxBound;

    float m_rotSpeed = 0.0001f;
    float m_speedMultiplier = 0.001f;
    float m_fieldOfView = 45.0f;

    std::vector<Star::ModelInstance> m_instances;
    std::vector<Uni::Math::Vector3f> m_velocities;
    Star::LightSource m_lightSource;

    Uni::Math::Matrix4x4f m_viewMatrix;
    Uni::Math::Matrix4x4f m_projectionMatrix;
};
