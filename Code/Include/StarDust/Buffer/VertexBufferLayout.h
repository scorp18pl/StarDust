#pragma once

#include <vector>

namespace Star
{
    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout();
        VertexBufferLayout(const VertexBufferLayout&) = default;
        VertexBufferLayout(VertexBufferLayout&&) = default;
        ~VertexBufferLayout() = default;

        template<typename T>
        void Push(unsigned int count);

        [[nodiscard]] const std::vector<VertexBufferElement>& GetElements()
            const;
        [[nodiscard]] unsigned int GetStride() const;

        VertexBufferLayout& operator=(const VertexBufferLayout&) = default;

    private:
        std::vector<VertexBufferElement> m_elements;
        unsigned int m_stride;
    };
} // namespace Star
