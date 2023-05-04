#pragma once

#include <StarDust/Model/Mesh.h>
#include <Universal/Math/Transform.h>
#include <string>

namespace Str
{
    struct InstanceData
    {
        Uni::Math::Matrix3x4f m_transform;
        Uni::Math::Matrix3x4f m_normalTransform;
        Uni::Grpx::Color m_color;
    };

    class ModelInstance
    {
    public:
        ModelInstance(
            std::string modelName,
            const Uni::Math::Transform& transform,
            const Uni::Grpx::Color& color);
        ModelInstance(
            PrimitiveType primitiveType,
            const Uni::Math::Transform& transform,
            const Uni::Grpx::Color& color);
        ModelInstance(ModelInstance&&) noexcept = default;
        ~ModelInstance() = default;

        [[nodiscard]] const std::string& GetModelName() const;
        [[nodiscard]] PrimitiveType GetPrimitiveType() const;
        [[nodiscard]] const Uni::Math::Transform& GetTransform() const;
        [[nodiscard]] const Uni::Grpx::Color& GetColor() const;

        Uni::Math::Transform& GetTransform();
        Uni::Grpx::Color& GetColor();

        ModelInstance& operator=(const ModelInstance&) = default;

    private:
        std::string m_modelName;
        PrimitiveType m_primitiveType{PrimitiveType::None};
        Uni::Math::Transform m_transform;
        Uni::Grpx::Color m_color;
    };
} // namespace Str
