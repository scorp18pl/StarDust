#include <StarDust/Buffer/VertexBufferLayout.h>
#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Utils.h>
#include <Universal/Math/Matrix/Matrix3x4f.h>
#include <Universal/Math/Vector/Vector3f.h>
#include <stdexcept>

namespace Star
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

    template<>
    void VertexBufferLayout::Push<Uni::Math::Vector3f>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, 3 * count, GL_FALSE });
        m_stride += 3 * Utils::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<Uni::Math::Vector4f>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, 4 * count, GL_FALSE });
        m_stride += 4 * Utils::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<Uni::Math::Matrix3x4f>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, 12 * count, GL_FALSE });
        m_stride += 12 * Utils::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<ModelInstanceData>(unsigned int count)
    {
        for (int i = 0; i < 3; ++i)
        {
            m_elements.push_back({ GL_FLOAT, 4, GL_FALSE });
            m_stride += 4 * Utils::GetSizeOfType(GL_FLOAT);
        }
        m_elements.push_back({ GL_FLOAT, 4, GL_FALSE });
        m_stride += 4 * Utils::GetSizeOfType(GL_FLOAT);
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
} // namespace Star