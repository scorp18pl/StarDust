#pragma once

namespace Star
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        VertexBuffer(const void* data, unsigned int size, bool isStatic = false);
        VertexBuffer(const VertexBuffer& other) = delete;
        VertexBuffer(VertexBuffer&&) noexcept ;
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, unsigned int size, bool isStatic = false);

        VertexBuffer& operator=(const VertexBuffer& other) = delete;

    private:
        unsigned int m_id;
    };
} // namespace Star
