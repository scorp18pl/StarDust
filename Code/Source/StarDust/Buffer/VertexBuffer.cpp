#include <StarDust/Buffer/VertexBuffer.h>
#include <StarDust/Utils.h>
#include <glad/glad.h>

namespace Str
{
    VertexBuffer::VertexBuffer()
    {
        GL_CHECK(glGenBuffers(1, &m_id));
    }

    VertexBuffer::VertexBuffer(
        const void* data, unsigned int size, bool isStatic)
        : VertexBuffer()
    {
        SetData(data, size, isStatic);
    }

    VertexBuffer::~VertexBuffer()
    {
        GL_CHECK(glDeleteBuffers(1, &m_id));
    }

    void VertexBuffer::Bind() const
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    }

    void VertexBuffer::Unbind() const
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::SetData(
        const void* data, unsigned int size, bool isStatic)
    {
        Bind();
        GL_CHECK(glBufferData(
            GL_ARRAY_BUFFER,
            size,
            data,
            isStatic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }
} // namespace Str
