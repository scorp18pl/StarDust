#pragma once

#include <Universal/Graphics/Bitmap.h>

namespace Star
{
    class SpriteAsset
    {
    public:
        using IdType = size_t;
        static constexpr IdType InvalidId = -1;

        static IdType GetId(const std::string& name) { return std::hash<std::string>{}(name); }
        static SpriteAsset CreateFromFile(const std::string& name, const std::string& fileName);

        static std::string GetMeshName(IdType id) { return "Sprite_" + std::to_string(id); }
        static std::string GetMeshName(const std::string& spriteName)
        {
            return GetMeshName(GetId(spriteName));
        }

        SpriteAsset(const std::string& name, const Uni::Grpx::Bitmap& bitmap);
        ~SpriteAsset() = default;

        [[nodiscard]] IdType GetId() const { return GetId(m_name); }
        [[nodiscard]] const std::string& GetName() const { return m_name; }
        [[nodiscard]] const Uni::Grpx::Bitmap& GetBitmap() const { return m_bitmap; }

    private:
        std::string m_name;
        Uni::Grpx::Bitmap m_bitmap;
    };
} // namespace Star
