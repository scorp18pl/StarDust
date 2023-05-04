#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector2f.h>
#include <vector>

namespace Str
{
    using Index = unsigned int;

    enum class PrimitiveType
    {
        None,
        Triangle,
        Rectangle,
        Box,
        Icosahedron,
    };

    struct Vertex
    {
        Uni::Math::Vector3f m_position{0.0f, 0.0f, 0.0f};
        Uni::Math::Vector3f m_normal{0.0f, 0.0f, 0.0f};
        Uni::Math::Vector2f m_texCoordinates{-1.0f, -1.0f};
    };

    struct Mesh
    {
        std::vector<Vertex> m_vertices;
        std::vector<Index> m_indices;
    };
} // namespace Str
