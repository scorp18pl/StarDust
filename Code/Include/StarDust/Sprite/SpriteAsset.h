#pragma once

#include <Universal/Graphics/Bitmap.h>
#include <string>
#include <unordered_map>

namespace Star
{
    struct SubSpriteData
    {
        size_t m_x, m_y, m_width, m_height;
        std::string m_name;
    };

    class SpriteAsset
    {
    public:
        using IdType = size_t;
        static constexpr IdType InvalidId = -1;

        static IdType GetId(const std::string& name) { return std::hash<std::string>{}(name); }
        static SpriteAsset CreateFromFile(const std::string& name, const std::string& fileName);

        static std::string GetSpriteMeshName(const std::string& spriteName)
        {
            return "Sprite_" + spriteName;
        }

        static std::string GetSubspriteMeshName(
            const std::string& spriteName, const std::string& subspriteName)
        {
            return GetSpriteMeshName(spriteName) + "_" + subspriteName;
        }

        //! Creates a sprite asset from a bitmap and a name.
        //! @param name Name of the sprite asset. The name should not contain the '_' character.
        SpriteAsset(const std::string& name, const Uni::Grpx::Bitmap& bitmap);
        ~SpriteAsset() = default;

        [[nodiscard]] IdType GetId() const { return GetId(m_name); }
        [[nodiscard]] const std::string& GetName() const { return m_name; }
        [[nodiscard]] const Uni::Grpx::Bitmap& GetBitmap() const { return m_bitmap; }
        [[nodiscard]] const std::unordered_map<size_t, SubSpriteData>& GetSubSprites() const
        {
            return m_subSprites;
        }

        //! Designates a sub-sprite from the sprite asset.
        //! This is used when a sprite asset contains multiple sprites (is a tilemap).
        //! @param x X coordinate of the sub-sprite.
        //! @param y Y coordinate of the sub-sprite.
        //! @param width Width of the sub-sprite.
        //! @param height Height of the sub-sprite.
        void DesignateSubSprite(SubSpriteData subSpriteData);

    private:
        std::string m_name;
        Uni::Grpx::Bitmap m_bitmap;
        std::unordered_map<size_t, SubSpriteData> m_subSprites;
    };
} // namespace Star
