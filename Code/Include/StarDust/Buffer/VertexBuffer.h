#pragma once

namespace Str
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        VertexBuffer(const void* data, unsigned int size, bool isStatic = false);
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, unsigned int size, bool isStatic = false);

        VertexBuffer& operator=(const VertexBuffer&) = delete;

    private:
        unsigned int m_id;
    };
} // namespace Str
