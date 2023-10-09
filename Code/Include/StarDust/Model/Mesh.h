#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector2f.h>
#include <string>
#include <vector>

namespace Star
{
    using Index = unsigned int;

    struct Vertex
    {
        Uni::Math::Vector3f m_position{ 0.0f, 0.0f, 0.0f };
        Uni::Math::Vector3f m_normal{ 0.0f, 0.0f, 0.0f };
        Uni::Math::Vector2f m_texCoordinates{ -1.0f, -1.0f };
    };

    struct Mesh
    {
        using IdType = size_t;
        static constexpr IdType InvalidId = -1;

        static IdType GetId(const std::string& name) { return std::hash<std::string>{}(name); }

        static Mesh ReadFromObjFile(const std::string& fileName);

        IdType GetId() const { return GetId(m_name); }

        void WriteToObjFile(const std::string& fileName);

        std::vector<Vertex> m_vertices;
        std::vector<Index> m_indices;
        std::string m_name;
    };
} // namespace Star
