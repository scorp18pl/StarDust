#pragma once

#include <StarDust/Drawable/Drawable.h>
#include <Universal/Graphics/Color.h>
#include <Universal/Math/Transform.h>

namespace Str
{
    class Geometry3D : public Drawable
    {
    public:
        Geometry3D(
            const Uni::Math::Transform& transform = Uni::Math::Transform());
        Geometry3D(const Geometry3D& shape3D) = default;
        Geometry3D(Geometry3D&& shape3D) = default;
        ~Geometry3D() = default;

        [[nodiscard]] const Uni::Math::Transform& GetTransform() const;
        [[nodiscard]] Uni::Math::Transform& GetTransform();
        const VertexList& GetVertices() override;
        const IndexList& GetIndices() override;
        void SetTransform(const Uni::Math::Transform& transform);

        Geometry3D& operator=(const Geometry3D& shape3D) = default;

    protected:
        VertexList m_vertices;
        IndexList m_indices;
        Uni::Math::Transform m_transform;
    };
} // namespace Str
