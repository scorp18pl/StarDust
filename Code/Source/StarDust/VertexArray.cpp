#include <StarDust/VertexArray.h>
#include <StarDust/Utils.h>

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
        const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
    {
        Bind();
        vertexBuffer.Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];
            GL_CHECK(glEnableVertexAttribArray(i));
            GL_CHECK(glVertexAttribPointer(
                i,
                element.count,
                element.type,
                element.normalized,
                layout.GetStride(),
                (const void*)offset));
            offset += element.count * Utils::GetSizeOfType(element.type);
        }
    }
} // namespace Str
