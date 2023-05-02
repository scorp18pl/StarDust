#include "StarDust/Drawable/Geometry3D.h"

namespace Str
{
    Geometry3D::Geometry3D(const Uni::Math::Transform& transform)
    {
        m_transform = transform;
    }

    const Uni::Math::Transform& Geometry3D::GetTransform() const
    {
        return m_transform;
    }

    Uni::Math::Transform& Geometry3D::GetTransform()
    {
        return m_transform;
    }

    void Geometry3D::SetTransform(const Uni::Math::Transform& transform)
    {
        m_transform = transform;
    }

    const VertexList& Geometry3D::GetVertices()
    {
        return m_vertices;
    }

    const IndexList& Geometry3D::GetIndices()
    {
        return m_indices;
    }
} // namespace Str