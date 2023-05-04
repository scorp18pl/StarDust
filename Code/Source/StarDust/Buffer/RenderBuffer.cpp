#include <StarDust/Buffer/RenderBuffer.h>
#include <StarDust/Utils.h>

namespace Str
{
    RenderBuffer::RenderBuffer()
    {
        GL_CHECK(glGenRenderbuffers(1, &m_id));
    }

    void RenderBuffer::Bind() const
    {
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_id));
    }

    void RenderBuffer::Unbind()
    {
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }

    unsigned int RenderBuffer::GetID() const
    {
        return m_id;
    }

    void RenderBuffer::SetStorage(
        GLenum internalFormat, unsigned int width, unsigned int height) const
    {
        Bind();
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height));
    }

    RenderBuffer::~RenderBuffer()
    {
        GL_CHECK(glDeleteRenderbuffers(1, &m_id));
    }
} // namespace Str