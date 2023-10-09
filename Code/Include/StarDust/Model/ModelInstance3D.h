#pragma once

#include <StarDust/Model/Model.h>
#include <StarDust/Model/ModelInstanceSystem.h>
#include <Universal/Math/Transform.h>
#include <string>

namespace Star
{
    class ModelInstance3D
    {
    public:
        ModelInstance3D(
            Mesh::IdType meshId,
            const Uni::Math::Transform& transform = Uni::Math::Transform(),
            const Uni::Grpx::Color& color = Uni::Grpx::Color::White);
        ModelInstance3D(const ModelInstance3D&);
        ModelInstance3D(ModelInstance3D&&) noexcept;
        ~ModelInstance3D();

        [[nodiscard]] Mesh::IdType GetMeshId() const { return m_meshId; }
        [[nodiscard]] const Uni::Math::Transform& GetTransform() const { return m_transform; }
        [[nodiscard]] const Uni::Grpx::Color& GetColor() const { return m_color; }

        Uni::Math::Transform& GetTransform() { return m_transform; }
        Uni::Grpx::Color& GetColor() { return m_color; }
        void SetMeshId(Mesh::IdType meshId);

        void Update();

        ModelInstance3D& operator=(const ModelInstance3D&);

    private:
        ModelInstance::IdType m_instanceId{ ModelInstance::InvalidId };
        Mesh::IdType m_meshId{ Mesh::InvalidId };

        Uni::Math::Transform m_transform;
        Uni::Grpx::Color m_color;
    };
} // namespace Star
