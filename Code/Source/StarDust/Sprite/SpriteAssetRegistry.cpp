#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Sprite/SpriteAssetRegistry.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace Star
{
    SpriteAssetRegistry::SpriteAssetRegistry() {}

    void SpriteAssetRegistry::RegisterSpriteAsset(const SpriteAsset& spriteAsset)
    {
        m_spriteAssets.emplace(spriteAsset.GetId(), spriteAsset);
    }

    void SpriteAssetRegistry::UnregisterSpriteAsset(const SpriteAsset& spriteAsset)
    {
        // TODO - Unregister sprite asset
    }

    void SpriteAssetRegistry::GenerateTextureAtlas()
    {
        std::vector<std::pair<size_t, SubspriteId>> spriteSizes = GetSortedSubspritesBySize();
        size_t totalSize = 0;
        for (auto& spriteSize : spriteSizes)
        {
            totalSize += spriteSize.first;
        }

        const auto sideLength = static_cast<size_t>(
            2.0f * 32.0f * std::ceil(static_cast<float>(std::sqrt(totalSize)) / 32.0f));

        m_spriteAtlas = Uni::Grpx::Bitmap(sideLength, sideLength, Uni::Grpx::Channel::Flags::Alpha);
        size_t atlasX = 0, atlasY = 0, maxHeight = 0;
        for (auto& spriteSize : spriteSizes)
        {
            const SpriteAsset& spriteAsset = m_spriteAssets.at(spriteSize.second.m_spriteId);
            const Uni::Grpx::Bitmap& spriteBitmap = spriteAsset.GetBitmap();
            size_t width = spriteBitmap.GetWidth(), height = spriteBitmap.GetHeight();
            size_t spriteX = 0, spriteY = 0;
            std::string subspriteName = SpriteAsset::GetSpriteMeshName(spriteAsset.GetName());
            if (spriteSize.second.m_isSubsprite)
            {
                const SubSpriteData& subsprite =
                    spriteAsset.GetSubSprites().at(spriteSize.second.m_subSpriteId);
                width = subsprite.m_width;
                height = subsprite.m_height;
                spriteX = subsprite.m_x;
                spriteY = subsprite.m_y;
                subspriteName =
                    SpriteAsset::GetSubspriteMeshName(spriteAsset.GetName(), subsprite.m_name);
            }

            maxHeight = std::max(maxHeight, height);
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
                subspriteName,
                static_cast<float>(width) / static_cast<float>(height),
                Uni::Math::BoundingBox2D::CreateFromDimensions(
                    Uni::Math::Vector2f{
                        static_cast<float>(atlasX),
                        static_cast<float>(atlasY),
                    } / static_cast<float>(sideLength),
                    Uni::Math::Vector2f{
                        static_cast<float>(width),
                        static_cast<float>(height),
                    } / static_cast<float>(sideLength))));

            m_spriteAtlas.WriteData(atlasX, atlasY, spriteBitmap, spriteX, spriteY, width, height);
            atlasX += width;
        }
    }

    std::vector<std::pair<size_t, SpriteAssetRegistry::SubspriteId>> SpriteAssetRegistry::
        GetSortedSubspritesBySize() const
    {
        std::vector<std::pair<size_t, SubspriteId>> subspriteSizes;
        for (auto& spriteAsset : m_spriteAssets)
        {
            const std::unordered_map<size_t, SubSpriteData>& subsprites =
                spriteAsset.second.GetSubSprites();
            if (subsprites.empty())
            {
                const size_t spriteSize = spriteAsset.second.GetBitmap().GetWidth() *
                    spriteAsset.second.GetBitmap().GetHeight();
                subspriteSizes.emplace_back(
                    spriteSize, SubspriteId{ .m_spriteId = spriteAsset.first });

                continue;
            }

            for (const auto& subsprite : subsprites)
            {
                const size_t subspriteSize = subsprite.second.m_width * subsprite.second.m_height;
                subspriteSizes.emplace_back(
                    subspriteSize,
                    SubspriteId{
                        .m_isSubsprite = true,
                        .m_spriteId = spriteAsset.first,
                        .m_subSpriteId = subsprite.first,
                    });
            }
        }

        std::sort(
            subspriteSizes.begin(),
            subspriteSizes.end(),
            [](auto& a, auto& b)
            {
                return a.first > b.first;
            });

        return subspriteSizes;
    }

    Mesh SpriteAssetRegistry::GenerateSpriteMesh(
        const std::string& spriteMeshName,
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
                    .m_texCoordinates = { spriteBoundingBox.GetMaxPoint().m_x,
                                          spriteBoundingBox.GetMinPoint().m_y },
                },
                Vertex
                {
                    .m_position = { 0.0f, -aspectRatio, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = { spriteBoundingBox.GetMinPoint().m_x,
                                          spriteBoundingBox.GetMaxPoint().m_y }
                },
                Vertex
                {
                    .m_position = { 1.0f, -aspectRatio, 0.0f },
                    .m_normal = Uni::Math::Vector3f::CreateAxisZ(),
                    .m_texCoordinates = spriteBoundingBox.GetMaxPoint(),
                },
            },
            .m_indices = {
                0, 1, 2, 1, 3, 2,
            },
            .m_name = spriteMeshName,
        };

        return spriteMesh;
    }
} // namespace Star