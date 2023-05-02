#include <StarDust/Drawable/Rectangle.h>

namespace Str
{
    const std::vector<unsigned int> Rectangle::Indices = { 0, 1, 2, 1, 2, 3 };

    Rectangle::Rectangle(
        const Uni::Math::Vector2f& dimensions,
        const Uni::Math::Matrix2x3f& transform,
        const Uni::Grpx::Color& color,
        float z_index)
        : m_positions{
            dimensions * -0.5f,
            { dimensions.m_x * 0.5f, dimensions.m_y * -0.5f },
            { dimensions.m_x * -0.5f, dimensions.m_y * 0.5f },
            dimensions * 0.5f,
        }
        , m_transform{transform}
    {
        m_vertices.reserve(CornerCount);

        for (const auto& position : m_positions)
        {
            m_vertices.push_back(Vertex{
                { position.m_x, position.m_y, z_index },
                color,
                { -1.0f, -1.0f },
            });
        }
    }

    Rectangle::Rectangle(Rectangle&& other) noexcept
        : m_positions{ other.m_positions }
        , m_transform{ other.m_transform }
        , m_vertices{ std::move(other.m_vertices) }
    {
    }

    void Rectangle::SetTransform(const Uni::Math::Matrix2x3f& transform)
    {
        for (size_t i = 0; i < CornerCount; ++i)
        {
            const auto& TransformedPosition = m_transform * m_positions[i];
            m_vertices[i].m_position = { TransformedPosition.m_x,
                                         TransformedPosition.m_y,
                                         m_vertices[i].m_position.m_z };
        }
        m_transform = transform;
    }

    const VertexList& Rectangle::GetVertices()
    {
        return m_vertices;
    }
    const IndexList& Rectangle::GetIndices()
    {
        return Indices;
    }

} // namespace Str