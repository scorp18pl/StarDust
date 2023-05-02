#pragma once

#include <glad/glad.h>
#include <vector>

namespace Str
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
        VertexBufferLayout(const VertexBufferLayout&) = delete;
        VertexBufferLayout(VertexBufferLayout&&) = delete;
        ~VertexBufferLayout() = default;

        template<typename T>
        void Push(unsigned int count);

        [[nodiscard]] const std::vector<VertexBufferElement>& GetElements()
            const;
        [[nodiscard]] unsigned int GetStride() const;

        VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;

    private:
        std::vector<VertexBufferElement> m_elements;
        unsigned int m_stride;
    };
} // namespace Str
