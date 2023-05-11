#pragma once

#include "Test.h"

#include <glm/mat4x4.hpp>

class InstancingTest : public Test
{
public:
    InstancingTest();

    void OnUpdate(float deltaTime) override;
    void OnRender(Str::Window& window) override;
    void OnImGuiRender() override;
private:
    static constexpr unsigned int InstanceCount = 100000U;
    const Uni::Math::Vector3f MaxBound = { 10000.0f, 10000.0f, 10000.0f };
    const Uni::Math::Vector3f MinBound = -MaxBound;

    float m_rotSpeed = 0.0001f;
    float m_speedMultiplier = 0.001f;
    float m_fieldOfView = 45.0f;

    std::vector<Str::ModelInstance> m_instances;
    std::vector<Uni::Math::Vector3f> m_velocities;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};
