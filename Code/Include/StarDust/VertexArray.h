#pragma once

#include <StarDust/Buffer/VertexBuffer.h>
#include <StarDust/Buffer/VertexBufferLayout.h>

namespace Str
{
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(const VertexArray&) = delete;
        VertexArray(VertexArray&&) = default;
        ~VertexArray();

        void Bind() const;
        static void Unbind();
        void AddBuffer(
            const VertexBuffer& vertexBuffer,
            const VertexBufferLayout& layout,
            unsigned int baseIndex = 0U);

        VertexArray& operator=(const VertexArray&) = default;
    private:
        unsigned int m_id;
    };
} // namespace Str
