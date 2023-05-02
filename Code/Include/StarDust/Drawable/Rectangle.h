#pragma once

#include <StarDust/Drawable/Drawable.h>
#include <Universal/Math/Matrix/Matrix2x3f.h>
#include <Universal/Math/Vector/Vector2f.h>

namespace Str
{
    class Rectangle : public Drawable
    {
    public:
        static const std::vector<unsigned int> Indices;
        static constexpr unsigned int CornerCount = 4;

        Rectangle(
            const Uni::Math::Vector2f& dimensions,
            const Uni::Math::Matrix2x3f& transform,
            const Uni::Grpx::Color& color = Uni::Grpx::Color::White,
            float z_index = 0.0f);

        Rectangle(const Rectangle& other) = default;
        Rectangle(Rectangle&& other) noexcept;
        ~Rectangle() = default;

        void SetTransform(const Uni::Math::Matrix2x3f& transform);

        Rectangle& operator=(const Rectangle& other) = default;

    protected:
        [[nodiscard]] const VertexList& GetVertices() override;
        [[nodiscard]] const IndexList& GetIndices() override;

    private:
        std::array<Uni::Math::Vector2f, CornerCount> m_positions;
        Uni::Math::Matrix2x3f m_transform;
        VertexList m_vertices;
    };
} // namespace Str
