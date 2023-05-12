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
            PrimitiveType primitiveType,
            const Uni::Math::Transform& transform = Uni::Math::Transform(),
            const Uni::Grpx::Color& color = Uni::Grpx::Color::White);
        ModelInstance(const ModelInstance&);
        ModelInstance(ModelInstance&&) noexcept;
        ~ModelInstance();

        [[nodiscard]] const std::string& GetModelName() const;
        [[nodiscard]] PrimitiveType GetPrimitiveType() const;
        [[nodiscard]] const Uni::Math::Transform& GetTransform() const;
        [[nodiscard]] const Uni::Grpx::Color& GetColor() const;

        Uni::Math::Transform& GetTransform();
        Uni::Grpx::Color& GetColor();
        void SetPrimitiveType(PrimitiveType primitiveType);

        void Update();

        ModelInstance& operator=(const ModelInstance&);

    private:
        int m_instanceId{ -1 };
        std::string m_modelName;
        PrimitiveType m_primitiveType{ PrimitiveType::None };

        Uni::Math::Transform m_transform;
        Uni::Grpx::Color m_color;
    };
} // namespace Str
