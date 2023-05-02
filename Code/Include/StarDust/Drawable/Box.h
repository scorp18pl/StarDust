#pragma once

#include <StarDust/Drawable/Geometry3D.h>

namespace Str
{
    class Box : public Geometry3D
    {
    public:
        Box(const Uni::Math::Transform& transform = Uni::Math::Transform(),
            const Uni::Grpx::Color& color = Uni::Grpx::Color::White);

        const VertexList& GetVertices() override;
    private:
        Uni::Grpx::Color m_color;

        static constexpr unsigned int VertexCount = 8U;
        static constexpr unsigned int IndexCount = 36U;
        static const Uni::Math::Vector3f TemplatePositions[VertexCount];
    };
} // namespace Str
