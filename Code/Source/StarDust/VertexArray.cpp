#include <StarDust/Utils.h>
#include <StarDust/VertexArray.h>

namespace Str
{
    VertexArray::VertexArray()
    {
        GL_CHECK(glGenVertexArrays(1, &m_id));
    }

    VertexArray::~VertexArray()
    {
        GL_CHECK(glDeleteVertexArrays(1, &m_id));
    }

    void VertexArray::Bind() const
    {
        GL_CHECK(glBindVertexArray(m_id));
    }

    void VertexArray::Unbind()
    {
        GL_CHECK(glBindVertexArray(0));
    }

    void VertexArray::AddBuffer(
        const VertexBuffer& vertexBuffer,
        const VertexBufferLayout& layout,
        unsigned int baseIndex)
    {
        Bind();
        vertexBuffer.Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); ++i)
        {
            unsigned int index = baseIndex + i;
            const auto& element = elements[i];
            GL_CHECK(glEnableVertexAttribArray(index));
            GL_CHECK(glVertexAttribPointer(
                index,
                element.count,
                element.type,
                element.normalized,
                layout.GetStride(),
                (const void*)offset));
            if (baseIndex != 0)
            {
                GL_CHECK(glVertexAttribDivisor(index, 1));
            }
            offset += element.count * Utils::GetSizeOfType(element.type);
        }

    }
} // namespace Str
