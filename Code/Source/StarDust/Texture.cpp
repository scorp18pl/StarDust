#include <StarDust/Texture.h>
#include <StarDust/Utils.h>
#include <glad/glad.h>

namespace Str
{
    Texture::Texture(const void* data, unsigned int width, unsigned int height)
        : m_data(data), m_width(width), m_height(height)
    {
        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

        GL_CHECK(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data));
        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }

    Texture::Texture(const Texture& other)
    {
        (*this) = other;
    }

    Texture::~Texture()
    {
        if (m_id != 0)
        {
            GL_CHECK(glDeleteTextures(1, &m_id));
        }
    }

    void Texture::Bind(unsigned int textureSlot) const
    {
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureSlot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    unsigned int Texture::GetID() const
    {
        return m_id;
    }

    void Texture::Unbind()
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }

    Texture& Texture::operator=(const Texture& other)
    {
        GL_CHECK(glGenTextures(1, &m_id));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_id));

        GL_CHECK(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            other.m_width,
            other.m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            other.m_data));
        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        return *this;
    }
} // namespace Str