#include <StarDust/Drawable/Box.h>

namespace Str
{
    const Uni::Math::Vector3f Box::TemplatePositions[] = {
        { -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f }, { -0.5f, 0.5f, -0.5f },
        { -0.5f, 0.5f, 0.5f },   { 0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f },
        { 0.5f, 0.5f, -0.5f },   { 0.5f, 0.5f, 0.5f },
    };

    Box::Box(
        const Uni::Math::Transform& transform, const Uni::Grpx::Color& color)
        : Geometry3D(transform)
        , m_color(color)
    {
        m_vertices.resize(VertexCount);
        for (unsigned int i = 0; i < VertexCount; ++i)
        {
            m_vertices[i].m_position =
                transform.GetMatrix() * TemplatePositions[i];
            m_vertices[i].m_color = color;
        }

        m_indices = {
            0, 1, 2, 1, 2, 3, 0, 1, 4, 1, 4, 5, 0, 2, 4, 2, 4, 6,
            3, 5, 7, 1, 3, 5, 2, 3, 6, 3, 6, 7, 4, 5, 6, 5, 6, 7,
        };
    }

    const VertexList& Box::GetVertices()
    {
        for (unsigned int i = 0; i < VertexCount; ++i)
        {
            m_vertices[i].m_position =
                m_transform.GetMatrix() * TemplatePositions[i];
        }

        return m_vertices;
    }

} // namespace Str
