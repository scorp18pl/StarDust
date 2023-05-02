#include <StarDust/Buffer/VertexBufferLayout.h>
#include <StarDust/Utils.h>
#include <stdexcept>

namespace Str
{
    VertexBufferLayout::VertexBufferLayout()
        : m_stride(0)
    {
    }

    template<typename T>
    void VertexBufferLayout::Push(unsigned int count)
    {
        throw std::runtime_error("Type not implemented.");
    }

    template<>
    void VertexBufferLayout::Push<float>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_stride += count * Utils::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void VertexBufferLayout::Push<unsigned int>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_stride += count * Utils::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void VertexBufferLayout::Push<unsigned char>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_stride += count * Utils::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements()
        const
    {
        return m_elements;
    }

    unsigned int VertexBufferLayout::GetStride() const
    {
        return m_stride;
    }
} // namespace Str