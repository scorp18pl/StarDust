#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector2f.h>
#include <vector>
#include <string>

namespace Star
{
    using Index = unsigned int;

    struct Vertex
    {
        Uni::Math::Vector3f m_position{0.0f, 0.0f, 0.0f};
        Uni::Math::Vector3f m_normal{0.0f, 0.0f, 0.0f};
        Uni::Math::Vector2f m_texCoordinates{-1.0f, -1.0f};
    };

    struct Mesh
    {
        using IdType = int;
        static constexpr IdType InvalidId = -1;

        static Mesh ReadFromObjFile(const std::string& fileName);

        void WriteToObjFile(const std::string& fileName);

        std::vector<Vertex> m_vertices;
        std::vector<Index> m_indices;
        std::string m_name;
    };
} // namespace Star
