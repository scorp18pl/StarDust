#pragma once

namespace Star
{
    class IndexBuffer
    {
    public:
        // Always Dynamic
        IndexBuffer();
        IndexBuffer(const unsigned int* data, unsigned int count);
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) noexcept;
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;
        [[nodiscard]] unsigned int GetCount() const;

        void SetData(const unsigned int* data, unsigned int count);

        IndexBuffer& operator=(const IndexBuffer&) = delete;

    private:
        unsigned int m_id;
        unsigned int m_count{ 0U };
    };
} // namespace Star
