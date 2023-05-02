#pragma once

#include <Universal/Graphics/Bitmap.h>
#include <string>
#include <glad/glad.h>

namespace Str
{
    class Texture
    {
    public:
        explicit Texture(const std::string& path);
        Texture(const Texture&) = delete; // TODO: Implement copy constructor
        Texture(Texture&&) noexcept;
        ~Texture();

        const Uni::Grpx::Bitmap& GetBitmap() const;

        void Bind(unsigned int textureSlot = 0U) const;
        static void Unbind();

    private:
        Uni::Grpx::Bitmap m_bitmap;
        unsigned int m_id;
    };
} // namespace Str
