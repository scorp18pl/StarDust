#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector2f.h>
#include <Universal/Math/Vector/Vector3f.h>
#include <vector>

namespace Str
{
    struct Vertex
    {
        Uni::Math::Vector3f m_position;
        Uni::Grpx::Color m_color{Uni::Grpx::Color::White};
        Uni::Math::Vector2f m_texCoordinates{-1.0f, -1.0f};
    };

    using Index = unsigned int;
    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<Index>;
} // namespace Str
