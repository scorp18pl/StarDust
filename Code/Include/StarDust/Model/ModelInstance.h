#pragma once

#include <StarDust/Model/Model.h>
#include <Universal/Math/Transform.h>
#include <string>

namespace Star
{
    struct ModelInstanceData
    {
        Uni::Math::Matrix3x4f m_transform;
        Uni::Math::Matrix3x4f m_normalTransform;
        Uni::Grpx::Color m_color;
    };

    class ModelInstance
    {
    public:
        using IdType = int;

        static constexpr IdType InvalidId = -1;

        ModelInstance(
            Mesh::IdType meshId,
            const Uni::Math::Transform& transform = Uni::Math::Transform(),
            const Uni::Grpx::Color& color = Uni::Grpx::Color::White);
        ModelInstance(const ModelInstance&);
        ModelInstance(ModelInstance&&) noexcept;
        ~ModelInstance();

        [[nodiscard]] Mesh::IdType GetMeshId() const;
        [[nodiscard]] const Uni::Math::Transform& GetTransform() const;
        [[nodiscard]] const Uni::Grpx::Color& GetColor() const;

        Uni::Math::Transform& GetTransform();
        Uni::Grpx::Color& GetColor();
        void SetMeshId(Mesh::IdType meshId);

        void Update();

        ModelInstance& operator=(const ModelInstance&);

    private:
        int m_instanceId{ -1 };
        Mesh::IdType m_meshId{ -1 };

        Uni::Math::Transform m_transform;
        Uni::Grpx::Color m_color;
    };
} // namespace Star
