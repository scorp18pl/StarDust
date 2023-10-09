#pragma once

#include "TestScene.h"
#include <Universal/Algorithm/Noise/PerlinNoiseGenerator2D.h>

class CubeGameTest : public TestScene
{
public:
    CubeGameTest();
    ~CubeGameTest() override;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    Uni::Alg::Noise::PerlinNoiseGenerator2D m_noiseGenerator;

    unsigned int m_width{1000}, m_height{1000};
};
