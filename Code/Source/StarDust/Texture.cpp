#include <StarDust/Texture.h>
#include <StarDust/Utils.h>
#include <glad/glad.h>

namespace Str
{
    Texture::Texture(const std::string& path)
        : m_bitmap{ Uni::Grpx::Bitmap::CreateFromFile(
              path, true, Uni::Grpx::Channel::Flags::Alpha) }
    {
        GL_CHECK(glGenTextures(1, &m_id));
        glBindTexture(GL_TEXTURE_2D, m_id);

        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CHECK(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            static_cast<GLsizei>(m_bitmap.GetWidth()),
            static_cast<GLsizei>(m_bitmap.GetHeight()),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            m_bitmap.GetData()));

        Unbind();
    }

    Texture::Texture(Texture&& other) noexcept
        : m_bitmap{ std::move(other.m_bitmap) }
        , m_id{ other.m_id }
    {
        other.m_id = 0;
        other.m_bitmap = {};
    }

    Texture::~Texture()
    {
        if (m_id != 0)
        {
            GL_CHECK(glDeleteTextures(1, &m_id));
        }
    }

    const Uni::Grpx::Bitmap& Texture::GetBitmap() const
    {
        return m_bitmap;
    }

    void Texture::Bind(unsigned int textureSlot) const
    {
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureSlot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    void Texture::Unbind()
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }
} // namespace Str