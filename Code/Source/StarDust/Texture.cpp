#include <StarDust/Texture.h>
#include <StarDust/Utilities/Utils.h>
#include <glad/glad.h>

namespace Star
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
        GL_CHECK(glDeleteTextures(1, &m_id));
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

    void Texture::SetData(
        const void* data, unsigned int width, unsigned int height)
    {
        m_data = data;
        m_width = width;
        m_height = height;

        Bind();
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

    Texture& Texture::operator=(const Texture& other)
    {
        if (this == &other)
            return *this;

        GL_CHECK(glGenTextures(1, &m_id));
        Bind();

        SetData(other.m_data, other.m_width, other.m_height);

        return *this;
    }
} // namespace Star