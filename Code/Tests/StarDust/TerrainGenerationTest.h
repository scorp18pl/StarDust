#pragma once

#include "TestScene.h"
#include <Universal/Algorithm/Noise/PerlinNoiseGenerator2D.h>
#include <StarDust/LightSource.h>

class TerrainGenerationTest : public TestScene
{
public:
    TerrainGenerationTest();
    ~TerrainGenerationTest() override;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    Uni::Alg::Noise::PerlinNoiseGenerator2D m_noiseGenerator;
    unsigned int m_width{100}, m_height{100};
    Star::LightSource m_lightSource;
    Star::Model m_model;
};
