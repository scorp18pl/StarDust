#include <StarDust/Sprite/SpriteAsset.h>

namespace Star
{
    SpriteAsset SpriteAsset::CreateFromFile(const std::string& name, const std::string& fileName)
    {
        return SpriteAsset(name, Uni::Grpx::Bitmap::CreateFromFile(fileName, false));
    }

    SpriteAsset::SpriteAsset(const std::string& name, const Uni::Grpx::Bitmap& bitmap)
        : m_name{ name }
        , m_bitmap{ bitmap }
    {
    }

    void SpriteAsset::DesignateSubSprite(SubSpriteData subSpriteData)
    {
        m_subSprites.emplace(GetId(subSpriteData.m_name), subSpriteData);
    }

} // namespace Star