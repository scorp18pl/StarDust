#pragma once

#include <StarDust/Model/Mesh.h>
#include <StarDust/Sprite/SpriteAsset.h>
#include <Universal/Graphics/Bitmap.h>
#include <Universal/Math/BoundingBox2D.h>
#include <unordered_map>

namespace Star
{
    class SpriteAssetRegistry
    {
    public:
        static SpriteAssetRegistry& Get()
        {
            static SpriteAssetRegistry s_instance;
            return s_instance;
        }

        const Uni::Grpx::Bitmap& GetSpriteAtlas() const { return m_spriteAtlas; }

        void RegisterSpriteAsset(SpriteAsset& spriteAsset);
        void UnregisterSpriteAsset(SpriteAsset& spriteAsset);
        void GenerateTextureAtlas(); // TODO - Rethink this

    private:
        std::unordered_map<SpriteAsset::IdType, SpriteAsset> m_spriteAssets;
        Uni::Grpx::Bitmap m_spriteAtlas;

        static Mesh GenerateSpriteMesh(
            const std::string& spriteName,
            float aspectRatio,
            const Uni::Math::BoundingBox2D& spriteBoundingBox);

        SpriteAssetRegistry();
    };
} // namespace Star
