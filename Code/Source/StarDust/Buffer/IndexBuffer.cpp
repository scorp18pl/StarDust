#include <StarDust/Buffer/IndexBuffer.h>
#include <StarDust/Utilities/Utils.h>
#include <glad/glad.h>

namespace Star
{
    IndexBuffer::IndexBuffer()
    {
        GL_CHECK(glGenBuffers(1, &m_id));
    }

    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
        : IndexBuffer()
    {
        SetData(data, count);
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : m_id(other.m_id)
        , m_count(other.m_count)
    {
        other.m_id = 0;
        other.m_count = 0;
    }

    IndexBuffer::~IndexBuffer()
    {
        if (m_id != 0)
        {
            GL_CHECK(glDeleteBuffers(1, &m_id));
        }
    }

    void IndexBuffer::Bind() const
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    }

    void IndexBuffer::Unbind() const
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    unsigned int IndexBuffer::GetCount() const
    {
        return m_count;
    }

    void IndexBuffer::SetData(const unsigned int* data, unsigned int count)
    {
        m_count = count;
        Bind();
        GL_CHECK(glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            count * sizeof(unsigned int),
            data,
            GL_STATIC_DRAW));
    }
} // namespace Star