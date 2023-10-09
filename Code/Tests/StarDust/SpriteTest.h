#pragma once

#include "Test.h"

#include <StarDust/Texture.h>
#include <StarDust/Sprite/SpriteInstance.h>

class SpriteTest : public Test
{
public:
    SpriteTest();
    ~SpriteTest() override = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    Star::LightSource m_lightSource;
    Uni::Math::Matrix4x4f m_viewMatrix;
    Uni::Math::Matrix4x4f m_projectionMatrix;
    Star::Texture m_texture;
    std::vector<Star::SpriteInstance> m_spriteInstances;
};
