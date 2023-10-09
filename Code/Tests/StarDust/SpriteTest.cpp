#include "SpriteTest.h"

#include <StarDust/Renderer.h>
#include <StarDust/Sprite/SpriteAsset.h>
#include <StarDust/Sprite/SpriteAssetRegistry.h>
#include <StarDust/Texture.h>
#include <StarDust/Utilities/Math.h>
#include <Universal/Graphics/Bitmap.h>

SpriteTest::SpriteTest()
    : Test(Test::TestType::Sprite)
    , m_viewMatrix{ Star::Utils::CreateLookAtMatrix(
          Uni::Math::Vector3f(1.0f, 1.0f, 1.0f),
          Uni::Math::Vector3f(1.0f, 1.0f, 0.0f),
          Uni::Math::Vector3f(0.0f, 1.0f, 0.0f)) }
    , m_projectionMatrix{ Star::Utils::CreateOrthographicProjectionMatrix(
          -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f) }
    , m_lightSource(
          Star::LightSourceType::Directional,
          Star::LightData{
              Uni::Math::Vector3f(0.0f, 0.0f, -1.0f),
              Uni::Grpx::Color::White,
          })
{
    Star::SpriteAssetRegistry& registry = Star::SpriteAssetRegistry::Get();
    Star::SpriteAsset stardust = Star::SpriteAsset::CreateFromFile(
        "StarDust",
        "/home/scorp/Documents/code/repos/StarDust/Resources/Stardust/"
        "Textures/stardust-logo.png");
    Star::SpriteAsset universal = Star::SpriteAsset::CreateFromFile(
        "Universal",
        "/home/scorp/Documents/code/repos/StarDust/Resources/Stardust/"
        "Textures/universal-logo.png");

    registry.RegisterSpriteAsset(stardust);
    registry.RegisterSpriteAsset(universal);

    registry.GenerateTextureAtlas();
    const Uni::Grpx::Bitmap& atlas = registry.GetSpriteAtlas();
    m_texture = Star::Texture(atlas.GetData(), atlas.GetWidth(), atlas.GetHeight());
    m_texture.Bind(1);

    Star::ShaderProgram& shader = Star::Renderer::Get().GetUsedShaderProgram();
    shader.SetUniform1i("u_texture", 1);
    shader.SetUniformMat4x4f("u_view", m_viewMatrix);
    shader.SetUniformMat4x4f("u_proj", m_projectionMatrix);

    m_spriteInstances.emplace_back(
        "StarDust", Uni::Math::Vector3f(0.0f, 0.0f, 0.0f));
    m_spriteInstances.emplace_back(
        "Universal", Uni::Math::Vector3f(1.0f, 1.0f, 0.0f));
}

void SpriteTest::OnUpdate(float deltaTime) { Test::OnUpdate(deltaTime);
}

void SpriteTest::OnRender(Star::Window& window) { Test::OnRender(window); }

void SpriteTest::OnImGuiRender() { Test::OnImGuiRender(); }
