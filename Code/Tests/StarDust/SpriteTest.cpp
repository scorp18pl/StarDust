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
          Uni::Math::Vector3f(0.0f, 0.0f, 1.0f),
          Uni::Math::Vector3f(0.0f, 0.0f, 0.0f),
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
    Star::SpriteAsset subsprites = Star::SpriteAsset::CreateFromFile(
        "SubSprites",
        "/home/scorp/Documents/code/repos/StarDust/Resources/Stardust/"
        "Textures/testSubsprites.png");
    subsprites.DesignateSubSprite({
        .m_x = 0,
        .m_y = 0,
        .m_width = 16,
        .m_height = 16,
        .m_name = "Mail",
    });
    subsprites.DesignateSubSprite({
        .m_x = 16,
        .m_y = 0,
        .m_width = 16,
        .m_height = 16,
        .m_name = "Dwarf",
    });
    subsprites.DesignateSubSprite({
        .m_x = 0,
        .m_y = 16,
        .m_width = 16,
        .m_height = 16,
        .m_name = "Donut",
    });

    registry.RegisterSpriteAsset(stardust);
    registry.RegisterSpriteAsset(universal);
    registry.RegisterSpriteAsset(subsprites);

    registry.GenerateTextureAtlas();
    const Uni::Grpx::Bitmap& atlas = registry.GetSpriteAtlas();
    m_texture = Star::Texture(atlas.GetData(), atlas.GetWidth(), atlas.GetHeight());
    m_texture.Bind(1);

    Star::ShaderProgram& shader = Star::Renderer::Get().GetUsedShaderProgram();
    shader.SetUniform1i("u_texture", 1);
    shader.SetUniformMat4x4f("u_view", m_viewMatrix);
    shader.SetUniformMat4x4f("u_proj", m_projectionMatrix);

    m_spriteInstances.push_back(Star::SpriteInstance::CreateSpriteInstance(
        "StarDust", Uni::Math::Vector3f(-1.0f, -0.5f, 0.0f)));
    m_spriteInstances.push_back(Star::SpriteInstance::CreateSpriteInstance(
        "Universal", Uni::Math::Vector3f(0.0f, -0.5f, 0.0f)));
    m_spriteInstances.push_back(Star::SpriteInstance::CreateSubSpriteInstance(
        "SubSprites", "Mail", Uni::Math::Vector3f(-1.5f, 0.5f, 0.0f)));
    m_spriteInstances.push_back(Star::SpriteInstance::CreateSubSpriteInstance(
        "SubSprites", "Dwarf", Uni::Math::Vector3f(-0.5f, 0.5f, 0.0f)));
    m_spriteInstances.push_back(Star::SpriteInstance::CreateSubSpriteInstance(
        "SubSprites", "Donut", Uni::Math::Vector3f(0.5f, 0.5f, 0.0f)));
}

void SpriteTest::OnUpdate(float deltaTime) { Test::OnUpdate(deltaTime);
}

void SpriteTest::OnRender(Star::Window& window) { Test::OnRender(window); }

void SpriteTest::OnImGuiRender() { Test::OnImGuiRender(); }
