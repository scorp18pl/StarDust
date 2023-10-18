#pragma once

#include <StarDust/Model/Mesh.h>
#include <StarDust/Sprite/SpriteAsset.h>
#include <Universal/Graphics/Bitmap.h>
#include <Universal/Math/BoundingBox2D.h>

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

        void RegisterSpriteAsset(const SpriteAsset& spriteAsset);
        void UnregisterSpriteAsset(const SpriteAsset& spriteAsset);
        void GenerateTextureAtlas(); // TODO - Rethink this

    private:
        struct SubspriteId
        {
            bool m_isSubsprite{ false };
            SpriteAsset::IdType m_spriteId;
            //! Index of the sub-sprite in the atlas. Set to -1 if the wholeSprite is considered.
            size_t m_subSpriteId;
        };

        std::unordered_map<SpriteAsset::IdType, SpriteAsset> m_spriteAssets;
        Uni::Grpx::Bitmap m_spriteAtlas;

        std::vector<std::pair<size_t, SubspriteId>> GetSortedSubspritesBySize() const;

        static Mesh GenerateSpriteMesh(
            const std::string& spriteMeshName,
            float aspectRatio,
            const Uni::Math::BoundingBox2D& spriteBoundingBox);

        SpriteAssetRegistry();
    };
} // namespace Star
