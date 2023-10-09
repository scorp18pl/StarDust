#include <StarDust/Sprite/SpriteAsset.h>

namespace Star
{
    SpriteAsset SpriteAsset::CreateFromFile(const std::string& name, const std::string& fileName)
    {
        return SpriteAsset(name, Uni::Grpx::Bitmap::CreateFromFile(fileName, true));
    }

    SpriteAsset::SpriteAsset(const std::string& name, const Uni::Grpx::Bitmap& bitmap)
        : m_name{ name }
        , m_bitmap{ bitmap }
    {
    }

} // namespace Star