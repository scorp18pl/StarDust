#pragma once

#include <StarDust/Utils.h>
#include <glad/glad.h>

namespace Str
{
    template<GLenum BufferType>
    class Buffer
    {
    public:
        Buffer();
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&&) = delete;
        ~Buffer();

        void Bind() const;
        void Unbind() const;

        unsigned int GetID() const;

        Buffer& operator=(const Buffer&) = delete;

    protected:
        unsigned int m_id;
    };

    template<GLenum BufferType>
    Buffer<BufferType>::Buffer()
    {
        GL_CHECK(glGenBuffers(1, &m_id));
        Bind();
    }

    template<GLenum BufferType>
    Buffer<BufferType>::~Buffer()
    {
        GL_CHECK(glDeleteBuffers(1, &m_id));
    }

    template<GLenum BufferType>
    unsigned int Buffer<BufferType>::GetID() const
    {
        return m_id;
    }

    template<GLenum BufferType>
    void Buffer<BufferType>::Bind() const
    {
        GL_CHECK(glBindBuffer(BufferType, m_id));
    }

    template<GLenum BufferType>
    void Buffer<BufferType>::Unbind() const
    {
        GL_CHECK(glBindBuffer(BufferType, 0));
    }
} // namespace Str
