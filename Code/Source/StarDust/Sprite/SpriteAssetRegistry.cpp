#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Sprite/SpriteAssetRegistry.h>
#include <complex>
#include <stdexcept>
#include <vector>

namespace Star
{
    SpriteAssetRegistry::SpriteAssetRegistry() {}

    void SpriteAssetRegistry::RegisterSpriteAsset(SpriteAsset& spriteAsset)
    {
        m_spriteAssets.emplace(spriteAsset.GetId(), spriteAsset);
    }

    void SpriteAssetRegistry::UnregisterSpriteAsset(SpriteAsset& spriteAsset)
    {
        // TODO - Unregister sprite asset
    }

    void SpriteAssetRegistry::GenerateTextureAtlas()
    {
        std::vector<std::pair<size_t, SpriteAsset::IdType>>
            spriteSizes; // Vector of pairs of sprite sizes and sprite ids
        spriteSizes.reserve(m_spriteAssets.size());
        size_t totalSize = 0;
        for (auto& spriteAsset : m_spriteAssets)
        {
            const size_t spriteSize = spriteAsset.second.GetBitmap().GetWidth() *
                spriteAsset.second.GetBitmap().GetHeight();
            totalSize += spriteSize;

            spriteSizes.emplace_back(spriteSize, spriteAsset.first);
        }

        std::sort(
            spriteSizes.begin(),
            spriteSizes.end(),
            [](auto& a, auto& b)
            {
                return a.first > b.first;
            });

        const auto sideLength = static_cast<size_t>(
            2.0f * 32.0f * std::ceil(static_cast<float>(std::sqrt(totalSize)) / 32.0f));

        m_spriteAtlas = Uni::Grpx::Bitmap(sideLength, sideLength, Uni::Grpx::Channel::Flags::Alpha);
        size_t atlasX = 0, atlasY = 0;
        size_t maxHeight = 0;
        for (auto& spriteSize : spriteSizes)
        {
            const SpriteAsset& spriteAsset = m_spriteAssets.at(spriteSize.second);
            const Uni::Grpx::Bitmap& spriteBitmap = spriteAsset.GetBitmap();

            maxHeight = std::max(maxHeight, spriteBitmap.GetHeight());

            if (atlasX + spriteBitmap.GetWidth() > m_spriteAtlas.GetWidth())
            {
                atlasX = 0;
                atlasY += maxHeight;
                if (atlasY > m_spriteAtlas.GetHeight())
                {
                    throw std::runtime_error("Texture atlas was too small.");
                }
                maxHeight = 0;
            }

            MeshRegistry::Get().RegisterMesh(GenerateSpriteMesh(
                spriteAsset.GetName(),
                static_cast<float>(spriteBitmap.GetWidth()) / spriteBitmap.GetHeight(),
                Uni::Math::BoundingBox2D::CreateFromDimensions(
                    Uni::Math::Vector2f{
                        static_cast<float>(atlasX),
                        static_cast<float>(atlasY),
                    } / static_cast<float>(sideLength),
                    Uni::Math::Vector2f{
                        static_cast<float>(spriteBitmap.GetWidth()),
                        static_cast<float>(spriteBitmap.GetHeight()),
                    } / static_cast<float>(sideLength))));

            for (size_t y = 0; y < spriteBitmap.GetHeight(); ++y)
            {
                for (size_t x = 0; x < spriteBitmap.GetWidth(); ++x)
                {
                    m_spriteAtlas.SetPixelColor(
                        atlasX + x, atlasY + y, spriteBitmap.GetPixelColor(x, y));
                }
            }

            atlasX += spriteBitmap.GetWidth();
        }
    }

    Mesh SpriteAssetRegistry::GenerateSpriteMesh(
        const std::string& spriteName,
        float aspectRatio,
        const Uni::Math::BoundingBox2D& spriteBoundingBox)
    {
        Mesh spriteMesh = {
            .m_vertices = {
                Vertex
                {
                    .m_position = { 0.0f, 0.0f, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = spriteBoundingBox.GetMinPoint(),
                },
                Vertex
                {
                    .m_position = { 1.0f, 0.0f, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = { spriteBoundingBox.GetMaxPoint().m_x, spriteBoundingBox.GetMinPoint().m_y },
                },
                Vertex
                {
                    .m_position = { 0.0f, aspectRatio, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = { spriteBoundingBox.GetMinPoint().m_x, spriteBoundingBox.GetMaxPoint().m_y }
                },
                Vertex
                {
                    .m_position = { 1.0f, aspectRatio, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = spriteBoundingBox.GetMaxPoint(),
                },
            },
            .m_indices = {
                0, 1, 2, 1, 3, 2,
            },
            .m_name = SpriteAsset::GetMeshName(spriteName),
        };

        return spriteMesh;
    }
} // namespace Star